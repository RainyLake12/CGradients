# CGradient

Terminal program which draws a gradient in the terminal. CMovingGradient draws a gradient animation.

Uses config files to let you change settings in ~/.cgradient.config (for cgradient) and ~/.cmovinggradients.config (for cmovinggradients). Config files use a key-value system in the format "\<key\> \<value\>"

Keys and values in ~/.cgradients.config :
- topleft rgb(\<red\>,\<green\>,\<blue\>) for rgb values for top left corner
- topright rgb(\<red\>,\<green\>,\<blue\>) for rgb values for top right corner
- bottomleft rgb(\<red\>,\<green\>,\<blue\>) for rgb values for bottom left corner
- bottomright rgb(\<red\>,\<green\>,\<blue\>) for rgb values for bottom right corner
- widthmultiply \<value\> to multiply the width of the gradiant
- heightmultiply \<value\> to multiply the height of the gradiant

Keys and values in ~/.cmovinggradiant.config
- topleft1 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for top left corner for the first frame
- topright1 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for top right corner for the first frame
- bottomleft1 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for bottom left corner for the first frame
- bottomright1 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for bottom right corner for the first frame
- topleft2 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for top left corner for the second frame
- topright2 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for top right corner for the second frame
- bottomleft2 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for bottom left corner for the second frame
- bottomright2 rgb(\<red\>,\<green\>,\<blue\>) for rgb values for bottom right corner for the second frame
- widthmultiply \<value\> to multiply the width of the gradiant
- heightmultiply \<value\> to multiply the height of the gradiant
- cyclemillis \<value\> to change the time it takes (in milliseconds) to do a full cycle from frame 1 to frame 2 and back