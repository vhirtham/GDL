"""Collection of functions related to x86 vector registers."""

from typing import List


def supported_registers() -> List:
    """Get a list of supported vector registers.

    Returns
    -------
    List:
        List with supported vector registers

    """
    return ["__m128d", "__m128", "__m256d", "__m256"]


def is_supported_register(register: str) -> bool:
    """Return 'True' if the passed string has the name of a supported vector register.

    Parameters
    ----------
    register:
        Name of a vector register type

    Returns
    -------
    bool:
        'True' if the passed string has the name of a supported vector register.
        'False' otherwise

    """
    return register in supported_registers()


def num_register_elements(register: str) -> int:
    """Get the number of vector register elements.

    Parameters
    ----------
    register:
        Name of a vector register type

    Returns
    -------
    int:
        Number of vector register elements

    """
    if not is_supported_register(register):
        raise ValueError(f'"{register}" is an invalid register type')

    register_sizes = {"__m128": 4, "__m128d": 2, "__m256": 8, "__m256d": 4}
    return register_sizes[register]
