import os
import sys

# Load the compiled extension
if sys.platform.startswith("win"):
    from . import *  # Windows: Imports `vrmlxpy.pyd`
elif sys.platform.startswith("linux"):
    from . import *  # Linux: Imports `vrmlxpy.so`

import vrmlxpy.vrmlxpy as _impl
sys.modules[__name__] = _impl