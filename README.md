# Blackjack
Textbased single-player Blackjack game with a betting system and one save profile. Player can keep playing as long as they still have points to bet with, the game profile will track points, wins, loss, player blackjacks, and busts.

## Starting the Game

New players will start with 1000 points to bet with.

1) Player first chooses the amount they want to bet.
2) Player and dealer will be given their first two cards. (Dealer will have one card that is visible and one card that is hidden)
3) Player can choose if they want to continue to draw cards with Y or N. If player goes over 21 then they bust. 
4) Once player finishes but not bust, the dealer will start drawing their cards until they get close to 21 or bust.
5) If no one busts and there is no ties then the higher scoring entity wins.
6) If player wins by having a higher score then they get they add 100% (150% if blackjack) of their bet to their existing points.
7) Choose to continue or stop.

Note: When a player draws an ace(1), they can choose whether or not they want the card to be 1 or 11 during each step of the round.

## Terminology

**Bankroll:** The amount of points the player currently has.  
**Wins:** The number of wins by scoring higher points but not blackjack.  
**Loses:** The number of times the dealer wins by scoring higher points.  
**Blackjacks:** The number of wins by scoring blackjacks.  
**Busts:** THe number of loss due to player going over 21.  
