# Contributing to Mario Maker

Contributors to Mario maker are welcome!

## Resources

Before you're want to apply your changes make you sure that, at least,
you know the basics of game dev. Especially is required that you know how 
the ECS pattern works, because this pattern is at the core of the game.

If you don't know how it works, here's there is a list resources:

- [wikipedia page](https://en.wikipedia.org/wiki/Entity_component_system)
- [evolve your hierarchy article](http://cowboyprogramming.com/2007/01/05/evolve-your-heirachy/)
- [redxdev/ECS docs](https://github.com/redxdev/ECS)

Indeed, If you want to learn about others design pattern/architecture which are suitable for 
game dev check out the fantastic [gameProgrammingPatterns e-book](https://gameprogrammingpatterns.com/contents.html).

### Game Resources

- [Tilesets](https://www.spriters-resource.com/nintendo_switch/supermariomaker2/)
- [Tools for making the background transparent](https://www.photopea.com/)
- [Enemies' behaviour](https://supermariomaker2.fandom.com/wiki/Category:Enemies)

## Code style

**todo**

## Todos

Here's I made a list of thing that I would like to implement. Enjoy!

Feel free also to add thing that you would like to see in the game.

- [ ] frame rate drops when there are too many entities
- [ ] State pattern for controlling the player and the enemies (https://gameprogrammingpatterns.com/state.html)
- [x] FlagSystem (for winning)
- [ ] Enemies to Implement
    - [x] Goomba
    - [x] Koopa Troopa
    - [x] Piranha plant
    - [ ] Browser
    - [ ] Baby Browser
    - [x] Tartosso
    - [x] Thwomp
    - [ ] bob-omb
    - [ ] Hammer Bro
    - [ ] Hammer Heavy Bro
- Enemies power ups:
  - [x] Parachute
  - [x] Mushroom
  - [ ] Wings 
- [ ] fix animation of brick breaking
- [ ] Sounds and music
- [ ] Bubble after a player die
- [ ] Controller for the second player (controller input not detected in linux by raylib)
- [ ] Make controls customizable
- [ ] Add the possibility of choosing between Mario and Luigi
- [ ] Add the possibility of choosing the map to play (if there are more than one)
- [ ] Documenting the code ?? (if it does make sense)
- [x] Improve kinetic-kinetic collision detection, possible solutions:
    - spacialHashMap ✓
    - event-driven collision using a priority-queue