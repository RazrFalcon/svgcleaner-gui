### 0.9.5
#### Added
 - Removal of the `transform` attribute with a default value.

#### Fixed
 - Attributes resolving during the `defs` regrouping.

### 0.9.4
#### Changed:
 - Documentation formatting.

#### Fixed:
 - Crash during `defs` processing.
 - Crash during `use` resolving.
 - Elements -> *Resolve 'use' elements* does not resolve used `use` now.

### 0.9.3
#### Added:
 - Output -> Number list separator
 - The `marker` attribute removing.
 - Quotes escape in attribute values.

#### Changed:
 - Attributes with `inherit` or `currentColor` values and without a proper parent is an error now.

#### Fixed:
 - Groups removing with transform and non-SVG child.
 - Transform to path applying when a path has a style defined in a parent element.
 - Multipass processing.
 - `marker` property resolving from CSS.
 - The value of the `unicode` attribute is always escaped now.
 - Fixed paths preprocessing.

### 0.9.2
#### Added:
 - Elements -> *Group elements by equal styles* can group two nodes now.
 - Elements -> *Group elements by equal styles* can group into `svg` node now.
 - `defs` element is always the first child of an `svg` now.

#### Fixed:
 - Groups ungrouping inside a switch element.
 - Complex groups ungrouping.
 - Elements -> *Group elements by equal styles* recursive processing.
 - Crash if path has an implicit MoveTo after ClosePath.
 - Attributes -> *Remove unresolved classes from `class` attributes* option had no effect.
 - HiDPI support in GUI.

### 0.9.1
#### Changed:
 - Unresolved inherited attributes will produce a warning and not error now

#### Fixed:
 - Text processing
 - `xml:space` processing
 - Parsing of the `transform` list separated by comma
 - Additional whitespace during ArcTo writing
 - Paths settings saving

### 0.9.0

#### Added:
 - Option changes are highlighted now
 - Main -> Copy on error
 - Main -> Allow bigger files
 - Paths -> Apply transformations to paths
 - Output -> Coordinates numeric precision
 - Output -> Properties numeric precision
 - Output -> Transforms numeric precision
 - Attributes -> *Remove text-related attributes if there is no text* removes `line-height` and `xml:space` now
 - Attributes -> *Join presentational attributes* supports *All* now
 - Output -> *Indention* supports *Tabs* now

#### Fixed:
 - `stroke-dasharray` and `stroke-dashoffset` processing during transformations applying

### 0.8.1

#### Added:
 - Elements -> *Group elements by equal styles* can group by `transform` now
 - `overflow` attribute cleaning

#### Fixed:
 - Incorrect grouping in Elements -> *Group elements by equal styles*
 - Processing gradients with one stop children

### 0.8.0

#### Added:
 - Elements -> Group elements by equal styles
 - Elements -> Resolve 'use' elements
 - Attributes -> Apply transformations to shapes
 - Paths -> Use implicit LineTo commands
 - Paths -> Convert path segments into shorter ones
 - Output -> Path's coordinates numeric precision
 - Support for *universal* and *id* CSS selectors
 - Updates checking support

#### Changed:
 - Default numeric precision is 12 instead of 8
 - Files TreeView is read-only during cleaning now
 - Stop button doesn't block GUI now

#### Removed:
 - Attributes -> Move presentational attributes to the parent group

### 0.7.1

#### Added:
 - Attributes -> *Remove unresolved classes from 'class' attributes* option
 - Changelog tab with this text

#### Changed:
 - Elements -> *Remove invisible elements* now removes default `feColorMatrix` element
 - gzip compression is enabled by default

#### Fixed:
 - CSS processing when style defined multiple times
 - `px` removing from the <list-of-lengths>

#### Removed:
 - Time column

### 0.7.0

 - Initial release
