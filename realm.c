
/*
Copyright (C) 2014  Frank Duignan

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "console.h"
#include "realm.h"
#include <stdlib.h> // contains definition for rand
#include <stdio.h> 	// contains definition for stdin
// Find types: h(ealth),s(trength),m(agic),g(old),w(eapon)
const char FindTypes[]={'h','s','m','g','w'};


// The following arrays define the bad guys and 
// their battle properies - ordering matters!
// Baddie types : O(gre),T(roll),D(ragon),H(ag)
const char Baddies[]={'O','T','D','H'};
// The following is 4 sets of 4 damage types
const byte WeaponDamage[]={10,10,5,25,10,10,5,25,10,15,5,15,5,5,2,10};
#define ICE_SPELL_COST 10
#define FIRE_SPELL_COST 20
#define LIGHTNING_SPELL_COST 30
#define MAGIC_DICE_COST 15
#define RAY_HANDS_COST 25
#define THUNDER_KICK_COST 35
//aditiional attacks
const byte MagicDiceDamage[]={10,20,5,0};
const byte RayDamage[]={10,20,5,0};
const byte ThunderKickDamage[]={20,10,5,0};
///////////////////////////////////////////////
const byte FreezeSpellDamage[]={10,20,5,0};
const byte FireSpellDamage[]={20,10,5,0};
const byte LightningSpellDamage[]={15,10,25,0};
const byte BadGuyDamage[]={10,10,15,5};
int GameStarted = 0;
tPlayer thePlayer;
tRealm theRealm;
void delay(int len);
unsigned range_rand(unsigned range)
{
	return rand() % (range+1);
}
void runGame(void)
{
	char ch;
	
	printString("Welcome to MicroRealms!");	
	showHelp();		
	while(GameStarted == 0)
	{
		
		showGameMessage("Press S to start a new game");
		ch = getUserInput();			
		
		if ( (ch == 'S') || (ch == 's') )
			GameStarted = 1;
	}

	initRealm(&theRealm);	
	initPlayer(&thePlayer,&theRealm);
	showPlayer(&thePlayer);
	showRealm(&theRealm,&thePlayer);
	showGameMessage("Press H for help");
	
	while (1)
	{
		ch = getUserInput();
		if (ch != '\033')
			ch = ch | 32; // enforce lower case
		switch (ch) {
			case 'h' : {
				showHelp();
				break;
			}
			case 'n' : {
				showGameMessage("North");
				step('n',&thePlayer,&theRealm);
				break;
			}
			case 's' : {
				showGameMessage("South");
				step('s',&thePlayer,&theRealm);
				break;
			}
			case 'e' : {
				showGameMessage("East");
				step('e',&thePlayer,&theRealm);
				break;
			}
			case 'w' : {
				showGameMessage("West");
				step('w',&thePlayer,&theRealm);
				break;
			}
			case '\033' : {	
				// An arrow on the keyboard is represented by three characteres: '\033' + '[' + {'A' or 'B' or 'C' or 'D'} depending on the direction of the arrow
				getchar();		// Skip the '['
				switch(getchar()){
					case 'A':	// 'A' represents the arrow up.
						showGameMessage("North");
						step('n',&thePlayer,&theRealm);
						break;
					case 'B':	// 'B' represents the arrow down.
						showGameMessage("South");
						step('s',&thePlayer,&theRealm);
						break;
					case 'C':	// 'C' represents the arrow right.
						showGameMessage("East");
						step('e',&thePlayer,&theRealm);
						break;
					case 'D':	// 'D' represents the arrow left.
						showGameMessage("West");
						step('w',&thePlayer,&theRealm);
						break;
					}
				break;
			}
			case '#' : {		
				if (thePlayer.wealth)		
				{
					showPlayer(&thePlayer);
					showRealm(&theRealm,&thePlayer);
					thePlayer.wealth--;
				}
				else
					showGameMessage("No gold!");
				break;
			}
			case 'p' : {				
				showPlayer(&thePlayer);
				break;
			}
			default:
				break;
		} // end switch
	} // end while
}
void step(char Direction,tPlayer *Player,tRealm *Realm)
{
	int new_x, new_y;
	new_x = Player->x;
	new_y = Player->y;
	byte AreaContents;
	switch (Direction) {
		case 'n' :
		{
			if (new_y > 0)
				new_y--;
			break;
		}
		case 's' :
		{
			if (new_y < MAP_HEIGHT-1)
				new_y++;
			break;
		}
		case 'e' :
		{
			if (new_x <  MAP_WIDTH-1)
				new_x++;
			break;
		}
		case 'w' :
		{
			if (new_x > 0)
				new_x--;
			break;
		}		
	}
	AreaContents = Realm->map[new_y][new_x];
	if ( AreaContents == '*')
	{
		showGameMessage("A rock blocks your path.");
		return;
	}
	Player->x = new_x;
	Player->y = new_y;
	int Consumed = 0;
	switch (AreaContents)
	{
		
		// const char Baddies[]={'O','T','B','H'};
		case 'O' :{
			showGameMessage("A smelly green Ogre appears before you");
			Consumed = doChallenge(Player,0);
			break;
		}
		case 'T' :{
			showGameMessage("An evil troll challenges you");
			Consumed = doChallenge(Player,1);
			break;
		}
		case 'D' :{
			showGameMessage("A smouldering Dragon blocks your way !");
			Consumed = doChallenge(Player,2);
			break;
		}
		case 'H' :{
			showGameMessage("A withered hag cackles at you wickedly");
			Consumed = doChallenge(Player,3);
			break;
		}
		case 'h' :{
			showGameMessage("You find an elixer of health");
			setHealth(Player,Player->health+10);
			Consumed = 1;		
			break;
			
		}
		case 's' :{
			showGameMessage("You find a potion of strength");
			Consumed = 1;
			setStrength(Player,Player->strength+1);
			break;
		}
		case 'g' :{
			showGameMessage("You find a shiny golden nugget");
			Player->wealth++;			
			Consumed = 1;
			break;
		}
		case 'm' :{
			showGameMessage("You find a magic charm");
			Player->magic++;						
			Consumed = 1;
			break;
		}
		case 'w' :{
			Consumed = addWeapon(Player,range_rand(MAX_WEAPONS-1)+1);
			showPlayer(Player);
			break;			
		}
		case 'X' : {
			// Player landed on the exit
			printString("A door! You exit into a new realm");
			setHealth(Player,100); // maximize health
			initRealm(&theRealm);
			showRealm(&theRealm,Player);
		}
	}
	if (Consumed)
		Realm->map[new_y][new_x] = '.'; // remove any item that was found
}
int doChallenge(tPlayer *Player,int BadGuyIndex)
{
	char ch;
	char Damage;
	const byte *dmg;
	int BadGuyHealth = 100;
	printString("Press F to fight");
	ch = getUserInput();
	if( ch == '\n')
		// Do nothing! Just consume the new line character!
	ch = getUserInput() | 32; // get user input and force lower case
	if (ch == 'f')
	{

		printString("Choose action");
		while ( (Player->health > 0) && (BadGuyHealth > 0) )
		{
			// Player takes turn first
			if (Player->magic > ICE_SPELL_COST)
				printString("(I)CE spell");
			if (Player->magic > FIRE_SPELL_COST)
				printString("(F)ire spell");
			if (Player->magic > LIGHTNING_SPELL_COST)
				printString("(L)ightning spell");
			if (Player->magic > MAGIC_DICE_COST)
				printString("(M)agic Dice");
			if (Player->magic > RAY_HANDS_COST)
				printString("(R)ay Hands");
			if (Player->magic > THUNDER_KICK_COST)
				printString("(T)hunder KIck");
			if (Player->Weapon1)
			{
				eputs("(1)Use ");
				printString(getWeaponName(Player->Weapon1));
			}	
			if (Player->Weapon2)
			{
				eputs("(2)Use ");
				printString(getWeaponName(Player->Weapon2));
			}
			printString("(P)unch");
			ch = getUserInput();
			if( ch == '\n')
				// Do nothing! Just consume the new line character!
			ch = getUserInput();
			switch (ch)
			{
				case 'i':
				case 'I':
				{
					printString("FREEZE!");
					Player->magic -= ICE_SPELL_COST;
					BadGuyHealth -= FreezeSpellDamage[BadGuyIndex]+range_rand(10);
					zap();
					break;
				}
				case 'f':
				case 'F':
				{
					printString("BURN!");
					Player->magic -= FIRE_SPELL_COST;
					BadGuyHealth -= FireSpellDamage[BadGuyIndex]+range_rand(10);
					zap();
					break;
				}
				case 'l':
				case 'L':
				{
					printString("ZAP!");
					Player->magic -= LIGHTNING_SPELL_COST;
					BadGuyHealth -= LightningSpellDamage[BadGuyIndex]+range_rand(10);
					zap();
					break;
				}
				case 'm':
				case 'M':
				{
					printString("Magic Dice");
					Player->magic -= MAGIC_DICE_COST;
					BadGuyHealth -= MagicDiceDamage[BadGuyIndex]+range_rand(10);
					zap();
					break;
				}
			    	case 'r':
				case 'R':
				{
					printString("RAY!");
					Player->magic -= RAY_HANDS_COST;
					BadGuyHealth -= RayDamage[BadGuyIndex]+range_rand(10);
					zap();
					break;
				}
			        case 't':
				case 'T':
				{
					printString("Thunder Kick!");
					Player->magic -=THUNDER_KICK_COST;
					BadGuyHealth -= ThunderKickDamage[BadGuyIndex]+range_rand(10);
					zap();
					break;
				}
				case '1':
				{
					dmg = WeaponDamage+(Player->Weapon1<<2)+BadGuyIndex;
					printString("Take that!");
					BadGuyHealth -= *dmg + range_rand(Player->strength);
					setStrength(Player,Player->strength-1);
					break;
				}
				case '2':
				{
					dmg = WeaponDamage+(Player->Weapon2<<2)+BadGuyIndex;
					printString("Take that!");
					BadGuyHealth -= *dmg + range_rand(Player->strength);
					setStrength(Player,Player->strength-1);
					break;
				}
				case 'p':
				case 'P':
				{
					printString("Thump!");
					BadGuyHealth -= 1+range_rand(Player->strength);
					setStrength(Player,Player->strength-1);
					break;
				}
				default: {
					printString("You fumble. Uh oh");
				}
			}
			// Bad guy then gets a go 
			
			if (BadGuyHealth < 0)
				BadGuyHealth = 0;
			Damage = BadGuyDamage[BadGuyIndex]+range_rand(5);
			setHealth(Player,Player->health - Damage);
			
				printf("Health: You %d%%", Player->health);
				printf(", them %d%%\n", BadGuyHealth);

			eputs("\r\n");
		}
		if (Player->health == 0)
		{ // You died
			printString("You are dead. Press Reset to restart");
			while(1);
		}
		else
		{ // You won!
			Player->wealth = 50 + range_rand(50);			
			showGameMessage("You win! Their gold is yours");			
			return 1;
		}
		
	}
	else
	{
		showGameMessage("Our 'hero' chickens out");
		return 0;
	}
}
int addWeapon(tPlayer *Player, int Weapon)
{
	char c;
	eputs("You stumble upon ");
	switch (Weapon)
	{
		case 1:
		{	
			printString("a mighty axe");
			break;
		}
		case 2:
		{	
			printString("a sword with mystical runes");
			break;
		}
		case 3:
		{	
			printString("a bloody flail");
			break;
		}		
		default:
			printHex(Weapon);
	}
	if ( (Player->Weapon1) && (Player->Weapon2) )
	{
		// The player has two weapons already.
		showPlayer(Player);
		printString("You already have two weapons");		
		printString("(1) drop Weapon1, (2) for Weapon2, (0) skip");
		c = getUserInput();
		switch(c)
		{
			eputc(c);
			case '0':{
				return 0; // don't pick up
			}
			case '1':{
				Player->Weapon1 = Weapon;
				break;
			}
			case '2':{
				Player->Weapon2 = Weapon;
				break;
			}
		}
	}
	else
	{
		if (!Player->Weapon1)
		{
			Player->Weapon1 = Weapon;	
		}
		else if (!Player->Weapon2)
		{
			Player->Weapon2 = Weapon;
		}
	}	
	return 1;
}
const char *getWeaponName(int index)
{
	switch (index)
	{
		case 0:return "Empty"; break;
		case 1:return "Axe";break;
		case 2:return "Sword"; break;
		case 3:return "Flail"; break;
	}
}

void setHealth(tPlayer *Player,int health)
{
	if (health > 100)
		health = 100;
	if (health < 0)
		health = 0;
	Player->health = health;
	
}	
void setStrength(tPlayer *Player, byte strength)
{
	if (strength > 100)
		strength = 100;
	if (strength < 0)
		strength = 0;
	Player->strength = strength;
}
void initPlayer(tPlayer *Player,tRealm *theRealm)
{
	// get the player name
	int index=0;
	byte x,y;
	char ch=0;
	// Initialize the player's attributes
	eputs("Enter the player's name: ");
	ch=getchar();
	if( ch == '\n')
		// Do nothing! Just consume the new line character!
	ch=0;
	while ( (index < MAX_NAME_LEN) && (ch != '\n') && (ch != '\r'))
	{
		ch = getUserInput();
		if ( ch > '0' ) // strip conrol characters
		{
			
			Player->name[index++]=ch;
		}
	}
	Player->name[index]=0; // terminate the name
	setHealth(Player,100);
	Player->strength=50+range_rand(50);
	Player->magic=50+range_rand(50);	
	Player->wealth=10+range_rand(10);
	Player->Weapon1 = 0;
	Player->Weapon2 = 0;
	// Initialize the player's location
	// Make sure the player does not land
	// on an occupied space to begin with
	do {
		x=range_rand(MAP_WIDTH);
		y=range_rand(MAP_HEIGHT);
		
	} while(theRealm->map[y][x] != '.');
	Player->x=x;
	Player->y=y;
}
void showPlayer(tPlayer *thePlayer)
{
	eputs("\r\nName: ");
	printString(thePlayer->name);
	
	
	printf("Health: \t%d%%\n", thePlayer->health);
	printf("Strength: \t%d%%\n", thePlayer->strength);
	printf("Magic: \t\t%d%%\n", thePlayer->magic);
	printf("Wealth: \t%d%%\n", thePlayer->wealth);

	
	printf("Location: \t%d", thePlayer->x);
	printf("-x , %d", thePlayer->y);
	printf("-y");

	
	eputs("\r\nWeapon1 : ");
	printString(getWeaponName(thePlayer->Weapon1));
	eputs("Weapon2 : ");
	printString(getWeaponName(thePlayer->Weapon2));
}
void initRealm(tRealm *Realm)
{
	int x,y;
	int Rnd;
	// clear the map to begin with
	for (y=0;y < MAP_HEIGHT; y++)
	{
		for (x=0; x < MAP_WIDTH; x++)
		{
			Rnd = range_rand(100);
			
			if (Rnd >= 98) // put in some baddies
				Realm->map[y][x]=	Baddies[range_rand(sizeof(Baddies))];
			else if (Rnd >= 95) // put in some good stuff
				Realm->map[y][x]=	FindTypes[range_rand(sizeof(FindTypes))];
			else if (Rnd >= 90) // put in some rocks
				Realm->map[y][x]='*'; 
			else // put in empty space
				Realm->map[y][x] = '.';	
		}
	}
	
	// finally put the exit to the next level in
	x = range_rand(MAP_WIDTH);
	y = range_rand(MAP_HEIGHT);
	Realm->map[y][x]='X';
}
void showRealm(tRealm *Realm,tPlayer *thePlayer)
{
	int x,y;
	printString("The Realm:");	
	for (y=0;y<MAP_HEIGHT;y++)
	{
		for (x=0;x<MAP_WIDTH;x++)
		{
			
			if ( (x==thePlayer->x) && (y==thePlayer->y))
				eputc('@');
			else
				eputc(Realm->map[y][x]);
		}
		eputs("\r\n");
	}
	printString("\r\nLegend");
	printString("(T)roll, (O)gre, (D)ragon, (H)ag, e(X)it");
	printString("(w)eapon, (g)old, (m)agic, (s)trength");
	printString("@=You");
}
void showHelp()
{

	printString("\n** Help **");
	printString("Use the arrow keys to choose dirction");
	printString("Action: Up -> North, Right -> East, Down -> South, Left -> West");
	printString("Action: # -> show map (Cost: 1 gold piece)");
	printString("Action: H -> Help");
	printString("Action: P -> Player details\n");
	
}

void showGameMessage(char *Msg)
{
	printString(Msg);
	printString("Ready");	
}
char getUserInput()
{
	char ch = 0;	
	while ((ch == 0))
		ch = egetc();	
	return ch;
}

void zap()
{
	// do some special effect when someone uses a spell
}
