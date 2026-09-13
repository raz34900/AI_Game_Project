****Game Rules:****
***Game Environment and Map* -----------------------------**

• Central Arena: The game takes place in a maze containing rooms and corridors 
between them. 

• Obstacles: Various obstacles are scattered in the rooms serving as hiding areas - 
walls, trees, and rocks. 

• Collection Points: 2 ammunition depots (armory in black with an orange dot) and 2 
pharmacies (hospitals in white with a red '+') are scattered randomly on the map.

• Lakes: There are lakes on the map that slow down the movement speed of the 
characters passing through them. 

• Shield Houses (Camouflage): There are 2 special items on the map (round shields in 
gray with a black circle inside them). A player who reaches them turns green and gains 
immunity from hits for 10 seconds (can shoot, but cannot be hit). The shield disappears 
after it has been used. 


***Teams and Characters* ---------------------------------**

• Objective: 2 teams participate in the game, where the goal of each team is to eliminate 
the opposing team. 

• Team Composition: Each team consists of 4 characters: two fighters, one medic, and 
one supply soldier. 

• Personal Character: A random "character" (e.g., aggressive fighter vs. cautious 
fighter) is defined for each character, which makes them behave differently and affects 
the chances of winning. 

***Health and Ammunition Meters* -----------------------------------**

• Team Meters: Each team has meters displaying the amount of health and 
supplies remaining for each player in real-time during the game. 

• Personal Indicators: Above each player's head, there is an additional 
personal meter that helps visualize, using colors, how much health and supply 
is left in real-time. 

***Artificial Intelligence (AI) and Finite State Machine (FSM)* --------------------------**

• Decision Making: The characters are managed by a finite state machine (FSM) and 
make decisions based on their current state (ammunition, health, environment). 

• Aspirations (States): 

o Engaging the enemy and searching for the rival character. 
o Survival and escape (when health or ammunition runs out). 
o Turning to the supply soldier to replenish ammunition. 
o Turning to the medic to heal and improve health points. 

• Movement and Navigation: After selecting a target, the character navigates to it 
using the A* search algorithm. 

• Dynamic Safety Map: Within the rooms where fighting takes place, a dynamic safety 
map is generated that changes in real-time every time a character moves. Character 
movement in the room must consider this map. 

***Combat System and Weapons* -------------------------------** 

• Combat Rules: Fighting takes place when rival characters are in the same room. 
There is no friendly fire. The medic and supply soldier cannot shoot, but they can 
definitely get hit.

• Weapons: 

o Black bullet shot: hits and reduces 2 health points. 
o Green grenade: thrown, explodes, and reduces 8 health points. 
o Gray mine with a red dot: every fighter (4 in total) can place a mine in a 
strategically chosen location. An enemy who steps on it loses 20 health points. 
Logistics (Medic and Supply) 

• Inventory: Fighters have a limited inventory of health (HP) and ammunition.

• Inventory Renewal: The supply soldier and the medic must physically reach the 
hospital or armory to collect equipment. One arrival at a building grants them one 
refill ability (for one time) for a fighter who requests it. 

• Healing Limitations: The medic and supply soldier cannot heal or add life to 
themselves. 

***Game Ending Conditions (Win/Loss Conditions)* -----------------------**

• Classic Victory: When the two fighters of a certain team die, the second team wins 
automatically. 

• Technical Victory / Attrition: If a team's supply soldier dies and its fighters have no 
ammunition left, the game ends in victory for the second team. 

• Attrition Tiebreaker: In a case where a supply soldier dies and there is no supply 
left, the team that currently has more health points is the one that will win. 

<img width="374" height="310" alt="image" src="https://github.com/user-attachments/assets/3c62cfed-42e6-4221-ad8f-b54528b5687b" />
<img width="373" height="330" alt="image" src="https://github.com/user-attachments/assets/68f9fb3d-2ccc-46fc-afed-b0259af95e06" />



