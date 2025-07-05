<p align="center"> 
   <img align="center" height="100%" src="./docs/imgs/menu.png" alt="CRougeLite">
  <br/>
  <b>
    Purr-fectly Perilous Adventures
  </b> 
  <br/>
</p>

---

## <img align="center" height="50px" width="50px" src="https://media3.giphy.com/media/v1.Y2lkPTc5MGI3NjExbHdhc3NwZmNscnl0NXlic2t2MmV0MDV0dTh1Mm95MGJ1enFpNWI2MiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/Nx4C51UKPqEpsYDz58/giphy.gif"> Overview

**CRougeLite** is a 2D top-down shooter game where you play as a fancy cat knight who explores the dangerous dungeons and fights enemies to save the dungeon from the evil forces.

It was our way to learn C programming (hence the 'C' in the name) and game development in a pure way with out the help of any game engine or heavy dependence on external libraries. So all the systems except the rendering\* was handled by us, that includes (Physics, Assets, Animation, Input, etc.).

> [!note]
> \*The rendering is done using the amazing library [Raylib](https://www.raylib.com/)

> [!important]
> As our goal was to learn C and game dev, the code was our best attempt to write clean and readable code (at the time of development 😆), So feel free to give us your feedback and code review on both code quality and game design. You can be as harsh as you want, we are here to learn and improve. 😉

## <img align="center" height="50px" width="50px" src="https://media3.giphy.com/media/v1.Y2lkPTc5MGI3NjExdTludzY3dWUxNnMyNGI4aXh0ZjIzdW02MjBvdDY3aWtvazV1b3NlZSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/ZgPf9DizfjcQscj214/giphy.gif"> Screenshots

<p align="center"> 
   <img align="center" height="100%" src="./docs/imgs/gameplay.jpeg" alt="Gameplay">
  <br/>
  <b>
    Gameplay showing the player, enemies, health bar, inventory.
  </b> 
  <br/>
</p>

<p align="center"> 
   <img align="center" height="100%" src="./docs/imgs/debug1.jpeg" alt="R RougeLite">
  <br/>
  <b>
    Debug view showing the enemies ranges, enemies path calculated through A* algorithm, and the player position.
  </b> 
  <br/>
</p>

<p align="center"> 
   <img align="center" height="100%" src="./docs/imgs/debug2.jpeg" alt="R RougeLite">
  <br/>
  <b>
    Debug view showing the map colliders, and collectable items.
  </b> 
  <br/>
</p>

---

## <img align="center" height="50px" width="50px" src="https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExcXg4OHVrdXR6cTJsbDZlZmxibzA5ZTB3NnU5emM0MTBkZXp2M3hpbiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9dHM/Xc3yCprU7fAAbPue4K/giphy.gif"> Gameplay

[![Watch the demo game play](./docs/imgs/menu.png)](https://www.youtube.com/watch?v=XZyabeO-LcU)

<p align="center"> 
  <br/>
  <b>
    CLick the image to watch the gameplay demo.
  </b> 
  <br/>
</p>

---

## <img align="center" height="50px" width="50px" src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExY2R4ZWNkejJtNmoxeDB6eWI4Ymxvb2lnaGlxbDQxNWNydXJnNG44ZSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/3o7WIrwBp1FA3J92SY/giphy.gif"> Game Features

---

### <img align="center" height="50px" width="50px" src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExcW95emthMDFnbDl0NXpjNDZndWJqOTk2MWw0ejY5cXNmN2JlaGZvNiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/8LGTzSNEImecGNaa2g/giphy.gif"> 1. Character Controller

- Custom player controller that allows movement, dashes, shooting, and melee attacks.

---

### <img align="center" height="50px" width="50px" src="https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExamNtNWNscXlmcmg1YTFpMTVlNWJsOGNzbWo0dnJ3MjhtNHQ2OGc5MSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/xUOwG6S7APT47XeaRy/giphy.gif"> 2. Enemy AI + A\* Pathfinding

- Enemies has a range of sight in which they can see the player and chase them.
- They use A\* pathfinding algorithm to navigate through the map and reach the player.
- They can attack the player when they are close enough.

---

### <img align="center" height="50px" width="50px" src="https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExc210bHhkd29vNDBjajA1eGlqZ21lM3l3bGdoaWJvcmZod3pkYnN1cCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/NDqBIAjtA1Z72/giphy.gif"> 3. Physics System

- handles collisions of rigid bodies, ghost bodies, and other physics interactions.

> [!note]
> You can check the collision docs that we wrote here [Collision Docs](./docs/collision-system.md)

---

### <img align="center" height="50px" width="50px" src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExYmliNGprbG1panR0dHczMWxlZGxjZGk5MmI1cHNodHhzZXI4eGExbiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/Art6YZ7HfLhFiHPk2U/giphy.gif"> 4. Inventory System and pickups

- Collectable items that can be picked up by the player.
- Player can have multiple items in their inventory.
- once used the item will be removed from the inventory.
- Players can press `F` to use the item in their inventory or interact with the environment.

---

### <img align="center" height="50px" width="50px" src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExcWxyemo0eDA4c2hpbWxnNWt2OGk3eXF0eW9yMmpib2d1eThxemdxMyZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/xVQZmKwFcwAwg/giphy.gif"> 5. Smart optimized asset management and loading

- Assets are kept to a minimum in one atlas image.
- Atlas is loaded once at the start and freed when the game is closed.


![Asset Management Diagram](./docs/imgs/atlas.png)

---

### <img align="center" height="50px" width="50px" src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExcW95emthMDFnbDl0NXpjNDZndWJqOTk2MWw0ejY5cXNmN2JlaGZvNiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/8LGTzSNEImecGNaa2g/giphy.gif"> 6. Animation System

- Implemented custom sprite animation system.
- Added an animation controller and animation state machines to allow for complex animation scenarios.

> [!note]
> You can check the animation docs that we wrote here [Animation Docs](./docs/animation-system.md)

![Animation Diagram](./docs/imgs/animation-system.png)

---

### <img align="center" height="50px" width="50px" src="https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExcXg4OHVrdXR6cTJsbDZlZmxibzA5ZTB3NnU5emM0MTBkZXp2M3hpbiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9dHM/Xc3yCprU7fAAbPue4K/giphy.gif"> 7. Debug tools

- Our debug menu handles:
  - Showing and hiding textures
  - Showing and hiding texture bounds
  - Showing and hiding colliders (Hitboxes)
  - Showing A* pathfinding information
  - Show inventory
  - Enable and control music
  - Control zoom

---

### <img align="center" height="50px" width="50px" src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExNmUyNjdieWlubjFheXY4b3FxczdsZnM1ZG5reXJmYjJqaGRhZjUzaiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/cLBlxq3UPzkf7uHVNN/giphy.gif"> 8. Audio system (music and sound effects)

- Handles music and ambient
- Added sound effects with simple adaptive sound design
  - Most sound have random pitch shift to reduce restiveness.

---

### <img align="center" height="50px" width="50px" src="https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExMHNvZXZoZWR6eXVibm16aGgwczhrYWJnZDh4enBjYjc0dGdqNnU0dCZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/RjpwS3HEFqgdALfARj/giphy.gif"> 9. Map System (map, colliders, loading, etc.)

- Map system that handles loading and unloading maps.
- Map colliders that are used to detect collisions with the player and enemies.

---

### <img align="center" height="50px" width="50px" src="https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExdG9uaDdvZnY5a3BlOTFzM2t1N3VuZm9nZHZybjYxOXdoeXZvcHF1cSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/xUPGcdVkKb65JURmBW/giphy.gif"> 10. Map Editor (to create maps)

- A simple map editor to create maps.
- Allows you to place tiles, colliders, and items.
- Fully implemented in One html file with no dependencies.

---

## <img align="center" height="50px" width="50px" src="https://media1.giphy.com/media/v1.Y2lkPTc5MGI3NjExOHFlYW52N3lxeXlsdDhxY3dibmV0aXY2bWllbHducXVjOGg4YzV1MiZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9cw/jdV1qhKi6H6la/giphy.gif"> Building and Running

- Run the command one time

```bash
cmake -S . -B build
```

- then to build the game run

```bash
cmake --build build
```

- to run

```bash
./build/CRougeLite/CRougeLite
```
---


## Small Dev Logs

> [!note]
> We made these early stage of the development.

- Debug menu

https://github.com/CmpGameDevs/CRougeLite/assets/88613195/7f33f56e-48b9-44d7-b6b7-c17bb6e8cf0c

- bullet mechanism animation system map drawing

https://github.com/CmpGameDevs/CRougeLite/assets/88613195/b4f882e9-f777-4ef8-86b4-f07be2bca59c

- Enemies

https://github.com/CmpGameDevs/CRougeLite/assets/88613195/0253e1f0-a1e2-4e23-a90d-dfadfa1daea3

- Animation

https://github.com/CmpGameDevs/CRougeLite/assets/88613195/f2cdf4f8-d7e9-420a-a4d2-33d8139cf26e

- Basic char movement and bullets

https://github.com/CmpGameDevs/CRougeLite/assets/88613195/fe7c7aa0-e9f0-41f7-b0aa-a6ca7fd24e12

---

## <img align="center" width="70px" height="55px" src="https://media0.giphy.com/media/Xy702eMOiGGPzk4Zkd/giphy.gif?cid=ecf05e475vmf48k83bvzye3w2m2xl03iyem3tkuw2krpkb7k&rid=giphy.gif&ct=s"> Contributors

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/marwan2232004"><img src="https://avatars.githubusercontent.com/marwan2232004" width="150px;" alt=""/><br />
      <sub><b>Marwan Alhameedy</b></sub></a><br />
    </td>
    <td align="center">
      <a href="https://github.com/Mo2Hefny"><img src="https://avatars.githubusercontent.com/Mo2Hefny" width="150px;" alt=""/><br />
      <sub><b>Moamen Hefny</b></sub></a><br />
    </td>
    <td align="center">
      <a href="https://github.com/YoussefMehany"><img src="https://avatars.githubusercontent.com/YoussefMehany" width="150px;" alt=""/><br />
      <sub><b>Youssef Mehany</b></sub></a><br />
    </td>
    <td align="center">
      <a href="https://github.com/amir-kedis"><img src="https://avatars.githubusercontent.com/amir-kedis" width="150px;" alt=""/><br />
      <sub><b>Amir Kedis</b></sub></a><br />
    </td>
  </tr>
</table>

## 🔒 License 


This software is licensed under MIT License. See [License](LICENSE) for more information.