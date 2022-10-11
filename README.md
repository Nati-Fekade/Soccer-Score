# Mode 3 Game on GBA
---

## Soccer Score

I cannot think of a better name but this game is an implementation of all the things we have learned so far.
I have created a soccer field and the goal (puns intended) is for the player to cross the whole field without being touched by the defenders.

**If touched by the defenders, the player would respawn to its initial spot.**

Once after you cross the field, you will reach a ***safe zone*** where you are closer and have more chance to score. **BUT it is not mandatory for you to shoot in this safe zone. You can shoot anywhere in the field. *You hava a better chance to score here though.***

**If blocked by the goalkeeper:**
1. The opponent gets a point
2. The player/you will be respawned to your initial spot
3. The player's size would ***INCREASE*** making it hard to get through
4. The goalkeeper's height would also ***INCREASE***

---

## How to Play?

* You will be prompted by each *state* and the buttons to press to go to another *state* on the screen. Ex:
    * On the **START** state, press *start* key to start game
    * On the **GAME** state, press *select* key to pause
    * On the **PAUSE** state, press *select* key to continue or *start* key to restart
    * On the **WIN** press *start* key to restart
    * On the **LOSE** press *start* key to restart
* Use the *UP*, *DOWN*, *LEFT*, and *RIGHT* key to move accordingly
* Press *A* key to shoot the ball
* HOLD *B* to collect health
* Press *L* to respawn health and decrease opponent's score (explained below)
* You win if you **score** 5 times
* You lose if you get **blocked** 5 times
---

## Sound Features

There are sound effects added when:
    
* Player collides with defender
* Player scores a goal
* Goalkeeper blocks the ball
* When "health" only decreases the opponent's score

---
## Additional Features

Out of the three additional required mechanics, I have implement *"two and half"* of them:

  1. I created a tiny magenta square called "health" that will decrease the opponent's score by 1 ONLY if the opponent's score is greater than 2
    * To collect "health" we have to **hold** key "***B***" and move the player using the arrow keys to get to safety.
    * The "Safety Zone" is the player's side/right side.
    * Once after you cross that line **press** key "***L***"
      * This will decrease the opponent's score
      * Respawn the *health* to a random position on the field

**WARNING:** Do not risk going for the health if the opponent's score is not above 2. It will ONLY respawn the health without decreasing the opponent's score.

2. Both the player and goal keepers size will ***INCREASE*** if the goal keeper blocks the ball.
3. There is an "exclusion area" and it is the left side of the field where the player cannot cross. However, the "exclusion area" is stationary and it does not change *(hence 2 and half)*

---
