National University of Computer &
Emerging Sciences (Karachi Campus)
PROJECT REPORT
AI SNAKE GAME
BCS 2H
GROUP MEMBERS
24K-0522 JAYESHA YAMIN
24K-1019 UMAIMA KHURSHID
24K-0908 ZAHRA SIDDIQUI
Project Report: Snake Game Development with Graphical User
Interface (GUI)
The classic Snake Game has been a staple of gaming culture since its inception,
captivating players with its simple yet addictive gameplay. However, traditional
implementations of the game often fall short in providing a visually stimulating and
interactive experience. In response to this, our proposed project aims to bridge this gap by
developing an enhanced version of the Snake Game with a modern Graphical User
Interface (GUI).

The motivation behind this project stems from a desire to revitalize the classic Snake
Game for contemporary audiences. While the core mechanics of the game remain
timeless, we recognize the need to adapt to evolving gaming standards and cater to the
expectations of modern players. By introducing a GUI-based interface, we seek to elevate
the gaming experience, offering enhanced visuals, interactive elements, and customizable
features.

Our goal is not only to preserve the nostalgic appeal of the Snake Game but also to
breathe new life into it, making it accessible and enjoyable for players of all ages.
Through the incorporation of advanced graphics, dynamic animations, and user-friendly
interfaces, we aim to create a gaming experience that is both engaging and immersive.

Furthermore, by leveraging the capabilities of C++ and the Raylib library, we aspire to
showcase the potential of these technologies in game development. Through this project,
we hope to inspire fellow developers and enthusiasts to explore innovative ways of
enhancing classic games with modern technology.

In summary, our project aims to revitalize the Snake Game by combining its timeless
gameplay with appealing visuals and incorporating some new elements.

Background: Research & Project Selection
➢ Evolution of Snake Game:
The Snake Game has seen various adaptations over the years. The traditional Snake
games often lacked complexity and interactivity. This project aims to modernize the
classic Snake Game by introducing AI-based movement (using A*) and dynamic scoring
based on food types, thus making the game more engaging and challenging for players.

➢ Limitations of Traditional Implementations:
Traditional Snake implementations typically involve basic food consumption and score
tracking, with static gameplay and limited interaction. While these versions are
enjoyable, they do not incorporate AI or dynamic elements that could provide a more
complex, immersive experience.

➢ Identifying the Need for Enhancement:
In response to the need for more engaging and intelligent game designs, this project was
created to add an AI-driven approach to Snake, making the snake's movement smarter
with the A* algorithm. The introduction of food types with varying effects on the snake's
score and behavior enhances the depth and replayability of the game.

➢ Selection of Project Approach:
The approach selected was to implement a GUI-based Snake Game using C++ and
Raylib. AI integration through the A* algorithm was introduced to control the snake’s
movement, while the dynamic scoring system adds variety and strategy to the game.

Project Specification: Detailed Overview
➢ Programming Language: C++
C++ was chosen due to its flexibility, performance, and suitability for game development,
particularly for managing the AI, game loop, and dynamic memory required for real-time
updates.

➢ Library: Raylib
Raylib was selected for its ease of use and efficiency in handling graphical rendering and
user input, making it ideal for implementing the GUI and visual effects in the game.

➢ Operating System: Windows
The game is optimized for the Windows operating system, ensuring compatibility with a
broad audience of players.

➢ Graphics and Animation:
Using Raylib, the game features dynamic food items with different colors, each of which
modifies the snake’s behavior or score. The snake’s movement, food interactions, and
score updates are all animated for an engaging player experience.

➢ File Handling:
The scores and gameplay statistics, including food consumption data, are saved to a text
file, which can be accessed later to track performance across multiple sessions.

Solution Design: Project Detail, Functionality, and Features
➢ Project Detail:
The Smart Snake Game with AI and Dynamic Scoring is designed to offer an enhanced,
more challenging version of the classic Snake Game. By integrating AI, dynamic food
types, different speed modes, and randomized food spawning, the game provides an
unpredictable and engaging experience for the player. The randomness of food placement
is a core design element that encourages strategic thinking and adaptation, ensuring that
each playthrough feels unique.

➢ Functionality and Features
1. Graphical User Interface (GUI):
The game starts with a front page displaying the game rules, providing clear instructions
for the player, also allowing the user to choose between easy, medium and difficult
modes, which affects the speed of the snake. Once gameplay begins, the player controls

the snake, consuming various types of food that affect the score in different ways. The
game dynamically updates the player’s score, with the highest score visible in real-time.

2. Randomized Food Spawning:
A key feature of this version of the Snake Game is the random placement of food on the
game board. This randomness adds an element of surprise and challenges players to be
more adaptable in their movement and strategy. Players can no longer rely on the food
being in the same place each time, requiring them to think quickly and plan their moves
as they navigate the board. As well as, a random powerup spawns up every few seconds
randomly, so the player has to race against time to eat it to increase the score
significantly.

3. AI Integration (A* Pathfinding):
In the AI mode, the snake’s movement is controlled by an A* algorithm, which ensures
smarter decision-making, allowing the snake to autonomously navigate toward food
while avoiding collisions. This integration makes the game more challenging and
dynamic, especially with the added unpredictability of food placement.

4. Variations of Food:
Black food: Normal food that increases the snake's length and score.

Red food: Decreases the score by 1.

Orange food: Reduces the snake's size.

Blue food: Increases the score by 2 and the snake's size.

Powerup: Increases score by 5.

5. Scoreboard:
At the end of the game, a scoreboard appears showing detailed statistics: how many food
items were eaten, the types of food consumed, and the total score. This gives players
insight into their performance and adds a layer of depth to the game.

6. Closing Window:
A closing window appears with options to either return to the main menu or exit,
providing flexibility for the player at the end of each round.

Implementation and Testing
➢ Implementation:
The implementation process began with the creation of the base gameplay mechanics
(snake movement, food interactions). The A* algorithm was integrated for pathfinding
and smart decision-making. The GUI and scoring system were then developed, followed
by testing and refinement of the various game elements.

➢ Testing:
Testing was conducted iteratively to ensure that each feature worked as expected. Unit
tests were used to verify individual components (e.g., snake movement, food scoring),
while user testing helped identify bugs and areas for improvement. Performance tests
were also conducted to ensure smooth gameplay.

Project Breakdown Structure: Workload Distribution
➢ GUI Development:
Jayesha Yamin designed and implemented the core gameplay mechanics and overall
game structure, laying the foundation for a cohesive player experience. She developed a
reliable collision detection system and crafted an intuitive UI layout, ensuring smooth
interaction and visual clarity. Her work was central to the technical and interactive
aspects of the game.

➢ Game Mechanics Implementation:
Umaima Khurshid implemented AI integration using the A* pathfinding algorithm to
control the snake’s movement with precision. She also developed the food logic,
designing various food types that uniquely impacted the score and the snake’s behavior.
Her contributions added strategic depth and intelligent gameplay dynamics.

➢ Scoreboard and Closing Window:
Zahra Siddiqui developed the scoreboard system, which accurately tracked the player’s
score and food consumption throughout the game. She also designed the end-game
window, providing players with intuitive options upon game completion. Her work

enhanced the game’s feedback loop and overall user experience. Also added a display for
the highest score across all rounds to motivate the player to beat it.

Results:
The random spawning of food, paired with AI-controlled movement, adds a compelling
layer of complexity and excitement to the game. In addition to the complexity of
choosing whether you want to play in easy, medium, or difficult mode, each playthrough
becomes a unique experience, as players must adapt to ever-changing food locations.
This unpredictability challenges players to think on their feet and adjust strategies in real
time.

The integration of the A* pathfinding algorithm ensures the snake navigates intelligently,
making the gameplay feel smooth yet competitive. Different food items come with varied
effects on the snake’s behavior and score, adding depth and encouraging thoughtful
decisions.

Together, these elements create a gameplay loop that is both engaging and mentally
stimulating. Players are constantly balancing risk and reward, reacting quickly while
planning. This combination of randomness and strategic depth keeps the game fresh and
replayable. It transforms a simple concept into a dynamic and immersive experience.

Conclusion: Summary & Discussion
The AI Snake Game (ASPK) is a modern take on the classic snake game,
enhanced with intelligent features and a user-friendly design. It offers an
engaging gameplay experience, combines real-time feedback with smart
mechanics, and concludes with a clear summary of performance. The project
reflects thoughtful planning, efficient execution, and a balanced integration of
AI and interactive elements.
