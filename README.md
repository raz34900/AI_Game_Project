<img width="374" height="310" alt="image" src="https://github.com/user-attachments/assets/3c62cfed-42e6-4221-ad8f-b54528b5687b" />
<img width="373" height="330" alt="image" src="https://github.com/user-attachments/assets/68f9fb3d-2ccc-46fc-afed-b0259af95e06" />

# AI-Driven Tactical Combat Simulation

https://github.com/user-attachments/assets/62efe977-5970-41e9-b766-3ef6977258e4

## Tech Stack & Architecture
![C#](https://img.shields.io/badge/C%23-8A2BE2?style=for-the-badge&logo=c-sharp&logoColor=white)
![FSM](https://img.shields.io/badge/Finite_State_Machine-FF1493?style=for-the-badge)
![A* Algorithm](https://img.shields.io/badge/A*_Pathfinding-0000FF?style=for-the-badge)
![Dynamic Mapping](https://img.shields.io/badge/Dynamic_Safety_Maps-228B22?style=for-the-badge)

## Project Overview
A complex, autonomous 2D tactical combat simulation. Designed and programmed to explore advanced artificial intelligence in gaming, the project features fully autonomous Non-Player Characters (NPCs) that make real-time, life-or-death decisions based on their environment, health, ammunition, and distinct personality traits (e.g., Aggressive vs. Cautious).

As a Software Engineer and AI Program Manager, my focus was on building robust logical architectures, including custom pathfinding algorithms, dynamic threat assessment, and Finite State Machines (FSM).

---

## Artificial Intelligence & Navigation
The core of the simulation relies on a sophisticated AI architecture managing 8 independent agents:
* **Finite State Machine (FSM):** Characters dynamically switch between behavioral states based on real-time needs:
  * *Engage/Hunt:* Seeking the enemy when resources are optimal.
  * *Survive/Flee:* Retreating when health or ammo is critically low.
  * *Resupply:* Navigating to the Supply Soldier for ammunition.
  * *Heal:* Navigating to the Medic for health restoration.
* **A* Pathfinding:** Agents calculate the optimal route to their dynamic targets (enemies, medics, or loot) across a complex maze.
* **Dynamic Safety Mapping:** During room-based combat, a real-time safety grid is generated and updated with every character movement, forcing agents to calculate the safest positioning dynamically.

---

## Teams & Roles
Two opposing teams fight for dominance. Each team consists of 4 characters with strict logical roles. Friendly fire is disabled.

| Role | Qty | Primary Function | Limitations |
| :--- | :---: | :--- | :--- |
| **Fighters** | 2 | Frontline combatants executing tactical maneuvers and utilizing all weaponry. | Rely entirely on support roles for survival; limited inventory. |
| **Medic** | 1 | Logistics agent. Travels to Hospitals to collect health packs for fighters. | Cannot shoot; cannot heal themselves; vulnerable to attack. |
| **Supply** | 1 | Logistics agent. Travels to Armories to collect ammo for fighters. | Cannot shoot; cannot resupply themselves; vulnerable to attack. |

---

## Combat System & Weapons
Combat triggers automatically when opposing fighters occupy the same room. Fighters utilize a distinct arsenal:

*  **Standard Bullet (Black):** Base attack, reduces enemy health by 2 HP.
*  **Grenade (Green):** Thrown explosive, reduces enemy health by 8 HP.
*  **Tactical Mine (Gray/Red):** Placed strategically. Enemies stepping on it lose 20 HP. Each fighter can place exactly one mine per game.

---

## Environment & Interactive Elements
The arena is a complex maze of interconnected rooms and corridors featuring interactive tactical elements:
* **Obstacles:** Walls, trees, and rocks break line-of-sight and serve as cover.
* **Lakes:** Environmental hazards that significantly slow down movement speed.
* **Shield Houses (Camouflage):** 2 rare items (gray round shields). Reaching one grants the agent 10 seconds of invulnerability (turns green, can shoot but takes no damage). Consumed upon use.
* **Resource Nodes:** 2 Armories (Black/Orange) and 2 Hospitals (White/Red Cross). Medics and Supply Soldiers must physically reach these nodes to acquire a single refill charge.

---

## UI & Telemetry
The simulation features comprehensive real-time UI tracking for analytical observation:
* **Global Team Dashboard:** Displays aggregate health and supply metrics for both teams.
* **Overhead Personal Meters:** Each agent has dynamic, color-coded floating UI bars visualizing precise HP and ammo levels in real-time.

---

## Win/Loss Conditions (Game Logic)
The simulation ends autonomously based on specific logical triggers:
1. **Classic Victory:** Both opposing fighters are eliminated.
2. **Technical Attrition:** The opposing Supply Soldier is dead, AND the remaining opposing fighters have 0 ammunition.
3. **HP Tiebreaker:** If a technical attrition state is reached but both teams are out of ammo/supply, the team with the highest remaining total Health Points wins.

• Attrition Tiebreaker: In a case where a supply soldier dies and there is no supply 
left, the team that currently has more health points is the one that will win. 




