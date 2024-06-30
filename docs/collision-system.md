# How to use the collision system

> [!NOTE]
> note that this system is new
> so come here often to make sure nothing has changed in the api

## Table of content

- Collision System Phases
- API documentation
- How to use the collision system **SKIP to here if you are in a hurry**

## Collision System Phases

### Broad Phase Collision Detection
  - The first phase of the collision system.
  - The map is divided into cells.
  - Each cell holds an array of probably colliding entities.
  - The function loops on the entities and add each one to their cell's array.

### Narrow Phase Collision Detection
  - The second phase of the collision system.
  - The function loops on each cell's entities array.
  - Check if the objects are colliding with each other.
  - Handle any collision that occur by adding the collided entities to a hit struct.

---

## API documentation

### initHitObject

- Initialize a hit struct to be used for collision handling.
- `return`: The hit struct object.

```c
Hit initHitObject(void);
//---
Hit hit = initHitObject();
```

### addEntityToHitObject

- Add a collided entity to hit object.
- `param`: hit - Pointer to the hit object.
- `param`: entity - Pointer to the entity.
- `return`: false if the entity is already in the list, true otherwise.

```c
bool addEntityToHitObject(Hit *hit, Entity *entity);
//---
isAdded = addEntityToHitObject(&(a->entity.action->hit), b);
```

### clearHitObject

- Free used heap space.
- `param`: hit - Pointer to the hit object.

```c
void clearHitObject(Hit *hit);
//---
clearHitObject(&(combatAction->hit));
```

### broadPhaseCollision

- Handles the broad collision detection phase and calls the Narrow Phase Collision Detections.

```c
void broadPhaseCollision(void);
//---
broadPhaseCollision();
```

---

## ✨ How to use the collision system ✨

- The Broad Phase Collision Detection is called in every game tick.
- The Narrow Phase Collision Detection is called by the previous function.
- In every update function use the hit struct to handle the collided objects.

### Now for the fun the practical example

- let's look at the combat action objects as an example

1. We update the state of combat actions objects in `updateCombatActions()` and check the hit object of the actions object.
```c
void updateCombatActions()
{
  CombatAction *actions = gameState->combatActions;

  for (int i = 0; i < gameState->numOfCombatActions; i++)
  {
    checkHitObject(actions + i);
  }

  // other functions
}
```

2. Handle collided entities in `checkHitObject(CombatAction *action)`
```C
static void checkHitObject(CombatAction *action)
{
  Hit *hit = &(action->hit);
  if (hit->hitCount == 0) return;

  Entity *entities = hit->entities;
  for (int i = hit->checkedCount; i < hit->hitCount; i++)
  {
    resolveCombatActionCollision(action, entities + i);
    hit->checkedCount++;
  }
}
```

---

# 🎉 Congratulations 🎉
