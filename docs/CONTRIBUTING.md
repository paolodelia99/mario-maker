# Contributing to Mario Maker

Contributors to Mario maker are welcome!

## Resources

Before you're want to apply your changes make sure that, at least,
you know the basics of game dev. Especially is required that you know how 
the ECS pattern works because this pattern is at the core of the game.

If you don't know how it works, here's there is a list of resources:

- [wikipedia page](https://en.wikipedia.org/wiki/Entity_component_system)
- [evolve your hierarchy article](http://cowboyprogramming.com/2007/01/05/evolve-your-heirachy/)
- [redxdev/ECS docs](https://github.com/redxdev/ECS)

Indeed, If you want to learn about others design pattern/architecture which could be suitable for 
game dev check out the fantastic [gameProgrammingPatterns e-book](https://gameprogrammingpatterns.com/contents.html).

### Game Resources

- [Tilesets](https://www.spriters-resource.com/nintendo_switch/supermariomaker2/)
- [Tools for making the background transparent](https://www.photopea.com/)
- [Enemies' behavior](https://supermariomaker2.fandom.com/wiki/Category:Enemies)

## Code style

**todo**

## Todos

Here's I made a list of things that I would like to implement. Enjoy!

Feel free also to add features that you would like to see in the game.

- [ ] make parachute swings
- [ ] State pattern for controlling the player and the enemies (https://gameprogrammingpatterns.com/state.html)
- Systems to implement: 
  - [ ] Score System (in progress)
  - [ ] Sound & Music system
  - [x] FlagSystem (for winning)
- [ ] Enemies to Implement
    - [x] Goomba
    - [x] Koopa Troopa
    - [x] Piranha plant
    - [x] Bullet-Bill  
    - [ ] Browser
    - [ ] Baby Browser
    - [x] Tartosso
    - [x] Thwomp
    - [ ] bob-omb
    - [ ] Hammer Bro
    - [ ] Hammer Heavy Bro
- Enemies power-ups:
  - [x] Parachute
  - [x] Mushroom
  - [ ] Wings 
- [ ] fix animation of brick breaking
- [ ] Bubble after a player dies
- [ ] Controller for the second player (controller input not detected in Linux by Raylib)
- [ ] Make controls customizable with options
- [ ] Add the possibility of choosing between Mario and Luigi
- [ ] Add the possibility of choosing the map to play (if there are more than one)
- [ ] Documenting the code ?? (if it does make sense)
- [x] Improve kinetic-kinetic collision detection, possible solutions:
    - spacialHashMap ✓
    - event-driven collision using a priority-queue