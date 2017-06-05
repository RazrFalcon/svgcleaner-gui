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
