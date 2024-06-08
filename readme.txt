Important specifics about the engine:

The heaviest system in my project is the collision system. Everytime an object wants to move somewhere, it needs to check all the colliders
in the world. I tried to make this functionality as fast as possible by providing early outs all throughout the algorithm.
Another major aspect was how any object that needed it, kept track of all the colliders in the world. Getting these every update would be
incredibly expensive, since we're talking about a vector with more than 400 objects. I solved this by internally flagging if a collider got
added or destroyed. Users can then check this boolean to see if they need to update their locally stored vectors.

I used a state machine to implement AI behaviour, this proved exceedingly difficult to wrap my head around. Now that I have implemented this
once, I feel like I could make a second AI, the Fygars much faster if I had the time to do so.

I used an event systems that listened to particular events to handle communication between different gameObjects. I wish I had used more of
the Subject/Observer pattern, but it was too hazy for me to use with confidence.

If there's one thing I'm proud of, it's in the Wall.cpp, Update. You can see a vector of strings, these are the file names of the different tile textures. Below you can see a fancy string of compound if-statements checking if particular booleans are true or false, adding up to an integer.This integer would match the bitwise representation of the booleans if placed next to eachother. Now the textures in that vector are sorted to match which side should be open or not, so using the calculated integer, you can acquire the correct texture. I wrote down the bitewise representation next to each for clarification.

Overall I have learned that, however interesting this may be, engine programming is not me forte. I would much rather design games in an already established game engine.