# Asset Toolkit

1 ) Background creation (bmp mode | 3 4 & 5)
To create bitmapped backgrounds, in mode 3 4 & 5 use the following programs : 

	- usenti : create an image of the right size for the mode, export as bmp.
	- use either grit or AGBGFX to convert into a .c file in either full colour mode or palette mode (256 colours)

2 ) Sprite creation : 
To create sprite maps, it is a much more involved process, as a sufficiently powerful tool has yet to be found.

	- First use usenti to create either a spritemap in 256 colour mode, or singular sprites in 16 colour mode
	- then do the same as 1 ) with grit to write a .c for that specific sprite or sprite map
	
	- if you need a sprite map for 16 colour mode sprites, draw all the sprites then export as 16 colour mode, set all the attributes manually in the .c, and replace t