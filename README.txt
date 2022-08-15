CS101 Project README.txt file

Controls : A to go left, D to go right, W to shoot, Q to quit the game

Instructions : Your aim is to shoot all the bubbles using bullets, that are shot from the shooter. You can move the shooter as described in the controls. You also must not let the bubbles hit the shooter. If that occurs, your health will decrease by 1. If your health reaches 0, the game will end.

If you manage to clear all 3 levels, by shooting all the bubbles, then you will have 'won'.

There is also a time factor, in each level you must finish by the particular time specified, else the game will end.

Features implemented : 

Basic :

The bubble's movement has been modified to incorporate vertical motion such that the bubbles travel in a parabolic path. Also, elastic collisions have been implemented between the bubble and any wall.

When a bubble and a bullet collide, both of them dissappear.

Collisions between the bubble and the shooter are also noted.

Additional (extra) features :

Bubbles of different sizes have been implemented, and whenever a bullet collides with a bigger bubble, it splits into two smaller bubbles, which initially have opposite horizontal velocities.

There are three levels of the game, and each level becomes more and more difficult. The first level contains two small sized bubbles, the second level contains three bubbles of radius twice the initial radius, and the third level contains bubbles 4 times larger than the initial ones. They also have decreasing time, with the time for the first level being 25 and the time decreasing by 5 each level. Bubbles in each level also have different colours.

Counters for Score, Time and Health have also been added. Score is initialised to 0 and incremented whenever a bullet hits a bubble. Time depends on the level, and health starts from 3 and is decremented whenever a bubble collides with a shooter. When the health reaches 0, the game stops. To denote a collision between the shooter and the bubble, the shooter momentarily flashes blue and then (almost instantaneously goes back to green again.

Other modifications : 

Modified the bubble class by introducing a new instance variable 's_no' , which serves as a way to identify a particular bubble. This is useful in the calculation of the health, since a function that just checks whether a bubble and shooter intersect, cannot detect if it was the same bubble that remains in contact with the shooter, nor if it was the same bubble hitting the shooter in multple separate instances, nor can it tell that 3 different bubbles have intersected the shooter. 

With the label of the bubble, and with the help of a set keeping track of all the bubbles that are currently in contact with the shooter, it becomes trivial to update the health of the game.

In every for loop, if a bullet or a bubble is being erased, the counting variable should always be decremented by 1 in order to look at the element just after the deleted element, since if a for loop proceeds normally, it would go unchecked in that particular frame.

Link to the recording : https://drive.google.com/drive/folders/1qx0o10Lzl9CLY1gFwVj1uqN5uJtUG_E7?usp=sharing






