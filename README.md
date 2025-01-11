# Bomberman
The Bomberman game in C++, without use of SFML library

## Description
Bomberman Clone is a grid-based arcade game where the player strategically places bombs to destroy obstacles and defeat enemies. The game involves elements of timing, strategy, and survival. The player must avoid enemies and explosions while aiming to clear all enemies to win.

This is a simplified version of the classic Bomberman game. The features and details to be implemented are provided below.

## Gameplay
Player vs. Enemies and Obstacles

1. The game field is divided into rows and columns, represented by a 2D array.
2. The player is represented by 'P' and starts at a fixed position on the grid.
3. Enemies are represented by 'E' and are placed randomly at the beginning of the game.
4. Bombs are placed by the player using a key (e.g., spacebar) and represented as 'B' in the grid.
5. The bomb explodes after a 3-second timer, clearing destructible obstacles ('O') and damaging enemies ('E').
6. The explosion radius extends horizontally and vertically, marked temporarily as 'X'.
7. Obstacles are represented by:
   - 'O': Destructible blocks that disappear when hit by an explosion.
   - '#': Indestructible walls that block movement and explosions.
8. The player can move freely across empty spaces (' '), but their movement is restricted by obstacles, walls, and the grid boundaries.
Note: Can use different symbols for players etc  for  better visualization.

## Features to Implement
### Player Movement
   - The player moves using arrow keys.
   - Movement is restricted by obstacles ('O') and walls ('#').

### Bomb Placement
   - Bombs are placed at the player's position and detonate after a fixed delay.
   - Bomb explosions clear destructible obstacles and damage enemies.

### Enemy 
   - Enemies move randomly within the grid but avoid walls and obstacles..

### Explosion Logic
   - Bomb explosions have a fixed range and affect all entities within the radius (enemies, obstacles, player).
   - The explosion lasts for a short duration and then resets the affected cells to empty (' ').

### Power-ups (Bonus)
   - Add power-ups that grant abilities like increased speed, bomb range, or multiple bombs.

### Win and Lose Conditions
   - The player wins by eliminating all enemies.
   - The player loses if caught in an explosion or collides with an enemy.
### File Handling
   - Store the top 3 max score in a txt file
### Sound
   - Add sound effects for bomb planting and detonating.
### Stages
   - Create at least 2 stages

## Scoring
- Destroying enemies: 50 points per enemy
- Destroying obstacles: 10 points per obstacle
- Picking up power-ups: 20 points per power-up

## Other features
1. Multiple Levels: Increase the number of enemies and obstacles with each level.
2. Dynamic Obstacles: Introduce new obstacle types like moving blocks.
3. Multiplayer Mode: Allow two players to compete on the same grid.

## Legend:
- 'P': Player
- 'E': Enemy
- 'O': Destructible Obstacle
- '#': Indestructible Wall
- ' ': Empty Space
- '+': Power-Up (e.g., increased bomb range)

----------

## Current progress:
- Everything almost complete, game win condition remaining only
