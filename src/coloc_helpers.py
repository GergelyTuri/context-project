"""
Functions for colocalization analysis
"""

import base64
import xml.etree.ElementTree as ET
from io import BytesIO

import matplotlib.image as mpimg
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import plotly.express as px
import plotly.graph_objects as go
import seaborn as sns
from PIL import Image
from scipy.spatial.distance import cdist


def parse_xml(xml_file_path: str) -> pd.DataFrame:
    """
    Parses an XML file and returns a DataFrame containing marker data.

    Parameters:
    xml_file_path (str): The path to the XML file.

    Returns:
    pd.DataFrame: A DataFrame containing marker data with columns 'Type', 'MarkerX', and 'MarkerY'.
    """
    tree = ET.parse(xml_file_path)
    root = tree.getroot()

    # Create lists to store data
    types = []
    marker_x_values = []
    marker_y_values = []

    # Iterate through Marker elements
    for marker_type in root.findall(".//Marker_Type"):
        current_type = marker_type.find("Type").text
        for marker in marker_type.findall(".//Marker"):
            marker_x = marker.find("MarkerX").text
            marker_y = marker.find("MarkerY").text

            # Append data to lists
            types.append(current_type)
            marker_x_values.append(marker_x)
            marker_y_values.append(marker_y)

    # Create a DataFrame
    data = {"Type": types, "MarkerX": marker_x_values, "MarkerY": marker_y_values}
    df = pd.DataFrame(data)

    # Display the DataFrame
    return df


def calc_pairwise_dist(dataframe: pd.DataFrame):
    """
    Calculate and return the pairwise distances between points of Type 1 and their closest points of Type 2.

    Parameters:
    - dataframe (pd.DataFrame): The input DataFrame with columns 'Type', 'MarkerX', and 'MarkerY'.

    Returns:
    - pd.DataFrame: A DataFrame with the coordinates of each Type 1 point, its closest Type 2 point,
                    and the distance between them.
    """
    # Filter and select coordinates based on 'Type', converting directly to int
    type1_df = dataframe.loc[dataframe["Type"] == "1", ["MarkerX", "MarkerY"]].astype(
        int
    )
    type2_df = dataframe.loc[dataframe["Type"] == "2", ["MarkerX", "MarkerY"]].astype(
        int
    )
    # Calculate pairwise distances
    distances_matrix = cdist(type1_df.values, type2_df.values)

    # Find the closest Type 2 point for each Type 1 point
    closest_indices = np.argmin(distances_matrix, axis=1)

    # Create a new DataFrame with Type 1 and closest Type 2 coordinates
    df_closest = pd.DataFrame(
        {
            "Type1_MarkerX": type1_df["MarkerX"].values,
            "Type1_MarkerY": type1_df["MarkerY"].values,
            "Type2_MarkerX": type2_df.iloc[closest_indices, 0].values,
            "Type2_MarkerY": type2_df.iloc[closest_indices, 1].values,
        }
    )
    # Calculate distances and add a new column
    df_closest["Distance"] = np.sqrt(
        np.sum(
            (
                df_closest[["Type1_MarkerX", "Type1_MarkerY"]].values
                - df_closest[["Type2_MarkerX", "Type2_MarkerY"]].values
            )
            ** 2,
            axis=1,
        )
    )
    return df_closest


def plot_distances(
    dataframe: pd.DataFrame, distance: float = 15.0, ax=None, image_path=None
):
    """
    Plots distances on a given axis 'ax' over an optional background image specified by 'image_path'.

    Parameters:
        dataframe (pd.DataFrame): The input DataFrame containing the data to be plotted.
        distance (float, optional): The maximum distance for filtering the data. Defaults to 15.0.
        ax (matplotlib.axes.Axes, optional): The axis on which to plot the distances. If not provided, a new axis will be created.
        image_path (str, optional): The path to the background image to be displayed. If provided, the image will be shown before plotting the distances.

    Returns:
        matplotlib.axes.Axes: The axis object containing the plotted distances.
    """
    if ax is None:
        _, ax = plt.subplots()

    # If an image path is provided, display the image first
    if image_path:
        image = mpimg.imread(image_path)
        ax.imshow(image)
        ax.axis("off")  # Optionally hide the axis

    df_filtered = dataframe[dataframe["Distance"] < distance].copy()

    columns_to_convert = [
        "Type1_MarkerX",
        "Type1_MarkerY",
        "Type2_MarkerX",
        "Type2_MarkerY",
    ]
    for column in columns_to_convert:
        df_filtered.loc[:, column] = df_filtered[column].astype(float)

    # Create a scatter plot over the image with empty circle markers
    sns.scatterplot(
        x="Type1_MarkerX",
        y="Type1_MarkerY",
        data=df_filtered,
        ax=ax,
        marker="o",
        facecolors="none",
        edgecolors="k",
    )
    return ax


def convert_image(image_path: str):
    """
    Downsamples an image by a given factor. Helper function for Plotly visualization.

    Parameters:
    - image_path (str): The path to the input image.
    - factor (int): The downsampling factor. Default is 1.

    Returns:
    - np.ndarray: The downsampled image as a NumPy array.
    """
    img = Image.open(image_path)
    buffered = BytesIO()
    img.save(buffered, format="PNG")
    img_str = base64.b64encode(buffered.getvalue()).decode()
    data_uri = "data:image/png;base64," + img_str
    return img, data_uri


def plot_distances_plotly(
    dataframe: pd.DataFrame, distance: float = 15.0, image_path=None
):
    """
    Plots a scatter plot using Plotly library, with optional background image.

    Parameters:
    - dataframe (pd.DataFrame): The input dataframe containing the data to be plotted.
    - distance (float): The maximum distance for filtering the data. Default is 15.0.
    - image_path (str): The path to the background image. If provided, the image will be displayed as the background of the plot.

    Returns:
    - fig (go.Figure): The Plotly figure object representing the scatter plot.
    """
    df_filtered = dataframe[dataframe["Distance"] < distance]
    columns_to_convert = [
        "Type1_MarkerX",
        "Type1_MarkerY",
        "Type2_MarkerX",
        "Type2_MarkerY",
    ]
    df_filtered[columns_to_convert] = df_filtered[columns_to_convert].astype(float)

    # Initialize the figure
    fig = go.Figure()

    # If an image path is provided, display the image as the background
    if image_path:
        img, data_uri = convert_image(image_path)
        # Add the image to the figure
        fig.add_layout_image(
            dict(
                source=data_uri,
                xref="x",
                yref="y",
                x=0,
                y=0,
                sizex=img.width,
                sizey=img.height,
                sizing="stretch",
                opacity=1.0,
                layer="below",
            )
        )
        # Update axes to match the image dimensions
        fig.update_xaxes(
            range=[0, img.width], showline=False, showgrid=False, visible=False
        )
        fig.update_yaxes(
            range=[0, img.height],
            showline=False,
            showgrid=False,
            visible=False,
            autorange="reversed",
        )

    # Add the scatter plot
    fig.add_trace(
        go.Scatter(
            x=df_filtered["Type1_MarkerX"],
            y=df_filtered["Type1_MarkerY"],
            mode="markers",
            marker=dict(
                size=10,
                color="black",
                opacity=1,
                symbol="circle-open",
            ),
            name="Distances",
        )
    )
    return fig
