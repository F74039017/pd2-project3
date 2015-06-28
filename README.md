Project3
===========================
The program is Qt Gui Application<br />
There are two modes you can choose in menu.<br />
Rank won't evaporate after the program terminated. <br />
(You can change it during game too, but the current score will be discarded.)<br />
If you need to clear it, please delete the user.db file in same directory<br />

More detail for this project, please click [here](https://terry-alu.hackpad.com/Project3-hmz15DNLNZl)

Environment 
--------------------------
OS: ubuntu 14.04 LTS<br />
gcc version: 4.8.2<br />
QMake version: 3.0<br />
Qt version: 5.4.0<br />

Compilation 
--------------------------
`qmake` `make`<br />
OR<br/>
Use Qt creator to compile the project<br />

Architecture 
--------------------------
**Mainwindow**

	Hold central and menu bar.
	Handle modes, option and mute function settings for the game.
	Ask user to type userName.
	
**Game**

	A QGraphicsView derived class.
	Handle the game scene and index scene.
	
**IndexScene**

	Start scene after typing userName.
	Handle the rank scene.
	
**GameScene**

	Handle whole game of project3, including animation, gameover scene...etc.
	
**Icon**

	Hold type and sound object.
	It is a abstract class for other classes prefixing "Icon".

**Other classes prefix "Icon"**

	Override the setType() and setImage() method to play sound and glow.	
	
**Square**

	Contain QPropertyAnimation class to implement disappear and move animation. (Controled by gameScene)
	
Project Requirement
--------------------------
1.	Fundamental rule is get scores as high as possible during finite steps or time.

2.	Record the number of stars and scores.

3.	Show Stones, stars, scores, rule(in menu) and gameover scene.

4.	Polymorphism is implemented in Icon based classes including setImage() and setType() functions.

5.	Operator overloading is implemented in Square class including == and != and =

6.	Emit quit(int, int) signal when user quits game.

As for screen shots and UML diagrams, please look up my report.

Reference	
------------------------
[**Index background picture**]
(http://hdw.eweb4.com/out/786647.html)<br />
[**Square Photoshop tutorial**]
(https://www.youtube.com/watch?v=qaNTZyne5RY)<br />
[**Framework**]
(https://www.youtube.com/watch?v=8ntEQpg7gck&list=PLMgDVIa0Pg8WrI9WmZR09xAbfXyfkqKWy)<br />
[**Music**]
(http://maoudamashii.jokersounds.com/)<br />

Too many to list all here.<br />
My Icon and squares were designed by following youtube tutorial.<br />
As for Animation and some effects, look up [here](http://doc.qt.io/qt-5/widget-classes.html) and StackOverflow.<br /> 
