from ._wrapper import SamplableSet
from _SamplableSet import * # keep original object available
SamplableSet.__module__ = 'SamplableSet' # Completes the wrapping to be invisible
