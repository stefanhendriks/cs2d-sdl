Counter-Strike 2D
-----------------

Introduction
============


Disclaimer
==========
99.9% of the graphics are made by their respective owners. They are either only 'resized' or
adjusted to fit them in the game. No graphics are made by myself at all. All graphics can be
accessed with the delivered utility "GRABBER". 

By using this product you understand that anything that *could* happen is by YOUR responsibility.

Controls
========


Editing files
=============
.DAT files contain (mostly) graphics and other files that are used by the game.
You can create your own DAT files for textures but you can also modify the 'main datafile', 
thats 'graphics.dat'. This file is used for most fancy stuff; players, weapons, sprites, etc.
Although you cannot add graphics without recompiling the source, you _CAN_ replace graphics
without any hassle. Simply create your own BMP file, and replace it with the GRABBER utility.



TEXTURES
========
Texture .DAT files are 'processed'. The .DAT files contain complete textures for each entry;
this is being read and processed to chop into tiles and texture groups. You can add properties
to textures to make specific effects on the texture in the game. 

Property fields
---------------
Word		Description
REAC		How does texture react when walked upon it?



Each property field has a word; ie in the GRABBER utilty you ADD a property, you name it 'REAC'
and then fill in the other field with the information you want to give to the game. 

FIELDS per property:

Word		Fields	
REAC		WATER, DUST

In short, this tells the game how to react on this texture property. This also tells what sounds
to be made when walked upon it! 