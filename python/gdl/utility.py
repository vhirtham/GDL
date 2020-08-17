"""Collection of utility functions."""

import datetime
import os
import sys
from typing import List


def filter_matching_strings(list_of_strings: List, matches: List) -> List:
    """Return all strings from a list that contain all the passed substrings.

    Parameters
    ----------
    list_of_strings:
        List of strings
    matches:
        List of substrings

    Returns
    -------
    List:
        List of strings that contain all passed substrings

    """
    return [
        string
        for string in list_of_strings
        if all(match in string for match in matches)
    ]


def get_script_path() -> str:
    """Get the path of the current script file.

    Returns
    -------
    str:
        Path of the current script file
    """
    return os.path.dirname(os.path.realpath(sys.argv[0]))


def get_datetime_string() -> str:
    """Get the current datetime as compact string.

    Returns
    -------
    str:
        Datetime string
    """
    time = datetime.now()
    return time.strftime("%Y%m%d_%H%M%S")
