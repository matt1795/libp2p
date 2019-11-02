import sphinx_rtd_theme
# The `extensions` list should already be in here from `sphinx-quickstart`
extensions = [
# there may be others here already, e.g. 'sphinx.ext.mathjax'
    'breathe',
    'exhale',
    'sphinx_rtd_theme'
]

html_theme = "sphinx_rtd_theme"

# Setup the breathe extension
breathe_projects = {
    "libp2p": "./doxyoutput/xml"
}

breathe_default_project = "libp2p"

# Setup the exhale extension
exhale_args = {
    # These arguments are required
    "containmentFolder":     "./api",
    "rootFileName":          "library_root.rst",
    "rootFileTitle":         "libp2p C++ API",
    "doxygenStripFromPath":  "..",
    # Suggested optional arguments
    "createTreeView":        True,
    # TIP: if using the
    # sphinx-bootstrap-theme, you need
    # "treeViewIsBootstrap": True,
    "exhaleExecutesDoxygen": True,
    "exhaleDoxygenStdin":
    "INPUT = ../include"
}

# Tell sphinx what the primary language being documented is.
primary_domain = 'cpp'

# Tell sphinx what the pygments highlight language should be.
highlight_language = 'cpp'

