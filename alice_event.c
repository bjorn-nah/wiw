#include "huc.h"
#include "sprite.h"

#define DIR_LEFT 1
#define DIR_RIGHT 2

#define DECAL 8
#define SLOW 4

#incspr(alice,"alice.pcx",0,0,2,20);
#incpal(alicepal,"alice.pcx");

#incspr(lapin,"lapin.pcx",0,0,2,20);
#incpal(lapinpal,"lapin.pcx");

#incspr(ball,"ball.pcx",0,0,1,1);
#incpal(ballpal,"ball.pcx");

#incchr(scene_chr,"background_ground.pcx");
#incpal(scene_pal,"background_ground.pcx");
#incbat(scene_bat,"background_ground.pcx",0x1000,32,28);

int tics_alice, frame_alice, state_alice, tics_lapin, frame_lapin, state_lapin, counter;
int j1, j2, alice_x, alice_y, alice_v, lapin_x, lapin_y, lapin_v, ball_x, ball_y, ball_vect_x, ball_vect_y;
int point_alice, point_lapin, play_state;
char dir_alice, dir_lapin, walk_alice, walk_lapin;

main()
{
	
	spr_make(0,180,121,0x5000,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x64,0,1);
	set_sprpal(0,lapinpal,1);
	load_vram(0x5000,lapin,0x600);

	spr_make(1,52,121,0x6200,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_32x64,1,1);
	set_sprpal(1,alicepal,1);
	load_vram(0x6000,alice,0x1000);	

	spr_make(2,200,100,0x5900,FLIP_MAS|SIZE_MAS,NO_FLIP|SZ_16x16,2,1);
	set_sprpal(2,ballpal,1);
	load_vram(0x5900,ball,0x100);

	load_background(scene_chr,scene_pal,scene_bat,32,28);

	counter = 0;
	tics_alice = 0;
	frame_alice = 0;
	state_alice = 0;
	tics_lapin = 0;
	frame_lapin = 0;
	state_lapin = 0;

	alice_x = 52;
	lapin_x = 180;
	alice_y = 121;
	lapin_y = 121;
	alice_v = 0;
	lapin_v = 0;

	ball_x = 200;
	ball_y = 100;
	ball_vect_x = 0;
	ball_vect_y = 0;

	point_alice = 0;
	point_lapin = 0;
	play_state = 0;

	set_font_color(13, 0);
	load_default_font();

	put_number(point_alice, 2 , 6, 0);
	put_number(point_lapin, 2 , 24 , 0);

	for(;;)
	{
		vsync();
		srand(counter);		/* re-randomise le random! */
		j1 = joy(0);
		j2 = joy(1);
		
		player_machine();
		score();
		physique();
		
		/*
		put_number(counter, 5 , 5, 0);
		put_number(point_alice, 5 , 5, 1);
		put_number(point_lapin, 5 , 5, 2); */
		put_number(ball_vect_x, 5 , 5, 3);
		/* put_number(ball_vect_y, 5 , 5, 4);
		put_number(play_state, 5 , 5, 5);
		*/
		counter++;
		satb_update();
	}
}

player_machine()
{
/*
	0 - Repos
	1 - Marche
	2 - Appel saut
	3 - Saut
	4 - Chute
	5 - Reception
	6 - Toucher balle
*/

	spr_set(1);
	if ( state_alice == 0) player_alice_0();
	if ( state_alice == 1) player_alice_1();
	if ( state_alice == 2) player_alice_2();
	if ( state_alice == 3) player_alice_3();
	if ( state_alice == 4) player_alice_4();
	if ( state_alice == 5) player_alice_5();
	if ( state_alice == 6) player_alice_6();

	/*
	spr_set(0);
	if ( state_lapin == 0) then player_lapin_0();
	if ( state_lapin == 1) then player_lapin_1();
	if ( state_lapin == 2) then player_lapin_2();
	if ( state_lapin == 3) then player_lapin_3();
	if ( state_lapin == 4) then player_lapin_4();
	if ( state_lapin == 5) then player_lapin_5();
	if ( state_lapin == 6) then player_lapin_6();
	*/
}

player_alice_0()
{
	if ( tics_alice == 0 ) spr_pattern(0x6200);
	tics_alice = 1;
	if (j1 & JOY_LEFT)
	{
		/* spr_ctrl(FLIP_X_MASK,FLIP_X); */
		tics_alice = 0;
		frame_alice = 0;
		state_alice = 1;
		spr_pattern(0x6200);
		dir_alice = DIR_LEFT;
	}
	if (j1 & JOY_RGHT)
	{
		/* spr_ctrl(FLIP_X_MASK,NO_FLIP_X); */
		tics_alice = 0;
		frame_alice = 0;
		state_alice = 1;
		spr_pattern(0x6000);
		dir_alice = DIR_RIGHT;
	}
	if (j1 & JOY_A)
	{
		tics_alice = 0;
		frame_alice = 0;
		state_alice = 3;
		spr_pattern(0x6000);
	}
}

player_alice_1()
{
	walk_alice = 0;
	tics_alice++;
	if (tics_alice > 6)
	{
		tics_alice = 0;
		frame_alice++;
		if (frame_alice > 3) frame_alice = 0;
		if (frame_alice == 0) spr_pattern(0x6200);
		if (frame_alice == 1) spr_pattern(0x6000);
		if (frame_alice == 2) spr_pattern(0x6200);
		if (frame_alice == 3) spr_pattern(0x6400);
	}
	if (j1 & JOY_LEFT && dir_alice == DIR_LEFT)
	{
		walk_alice = 1;
	}
	if (j1 & JOY_RGHT && dir_alice == DIR_RIGHT)
	{
		walk_alice = 1;
	}
	if (j1 & JOY_A)
	{
		tics_alice = 0;
		frame_alice = 0;
		state_alice = 3;
		spr_pattern(0x6000);
	}
	if (walk_alice == 0)
	{
		tics_alice = 0;
		frame_alice = 0;
		state_alice = 0;
	}
	
	else
	{
		if(dir_alice == DIR_LEFT)
		{
			alice_x-=2;
		}
		else
		{
			alice_x+=2;
		}
	
	}
		
}

player_alice_2()
{
return;
}

player_alice_3()
{
	if (alice_y == 121) alice_v = 10;
	if (j1 & JOY_LEFT)
	{
		dir_alice = DIR_LEFT;
		alice_x-=2;
	}
	if (j1 & JOY_RGHT)
	{
		dir_alice = DIR_RIGHT;
		alice_x+=2;
	}
	if (alice_v == 0) state_alice = 4;
}

player_alice_4()
{
	if (j1 & JOY_LEFT)
	{
		dir_alice = DIR_LEFT;
		alice_x-=2;
	}
	if (j1 & JOY_RGHT)
	{
		dir_alice = DIR_RIGHT;
		alice_x+=2;
	}
	if (alice_y == 121)  state_alice = 0;
}

player_alice_5()
{
return;
}

player_alice_6()
{
	play_state = 1;
	if (tics_alice == 0) spr_pattern(0x6800);
	if (tics_alice == 4) spr_pattern(0x6600);
	tics_alice++;
	if (tics_alice > 8)
	{
		tics_alice = 0;
		state_alice = 0;
	}
}

physique()
{

	if (alice_x < -8) alice_x = -8;
	if (alice_x > 96) alice_x = 96;
	alice_y = alice_y - alice_v;
	if (alice_y < 121) alice_v--;
	if (alice_y > 121) alice_y = 121;
	spr_put(1,alice_x,alice_y, dir_alice);

	/* collisions balle fillet */
	rebond(4, 112, 121, 15);

	/* collisions joueurs */
	rebond(1, alice_x, alice_y, 15);
	rebond(0, lapin_x, lapin_y, 15);

	
	if (play_state == 1)
	{
		/* gestion verticale de la balle */
		ball_y = ball_y - (ball_vect_y/ SLOW);;
		ball_vect_y--; 

		/* gestion horizontale de la balle */
		ball_x = ball_x + (ball_vect_x / SLOW);
		if (ball_x > 248) ball_vect_x = -ball_vect_x;
		if (ball_x < -8) ball_vect_x = -ball_vect_x;
	}
	spr_put(2, ball_x, ball_y, 1);
}

rebond(sprite_num, truc_x, truc_y, vect)
int truc_x, truc_y, sprite_num, vect;
{
	int diff;
	diff = ball_x - DECAL - truc_x;
	if (diff > -20 & diff < 20 & ball_y > (truc_y + DECAL - 22))
	{
		ball_vect_y = vect * (SLOW/2);
		ball_vect_x = diff * (SLOW/2);
		/*
		if (sprite_num == 0) 
		{
			state_lapin = 6;
			tics_lapin = 0;
		}
		*/
		if (sprite_num == 1) 
		{
			state_alice = 6;
			tics_alice = 0;
		}
	}
}

score()
{
	if(point_alice == 0 & point_lapin == 0 & play_state == 0)
	{
		ball_x = 120;
		ball_y = 50;
		if ((j1 | j2) &  JOY_STRT)
		{
			ball_vect_x = (random(21) - 10) * (SLOW/2);
			ball_vect_y = 15;
			play_state = 1;
		}
	}
	else

	if (ball_y > 150) 
	{
		ball_vect_x = 0;
		ball_vect_y = 0;
		ball_y = 80;
		play_state = 0;

		/* comptage points */
		if(ball_x < 128) 
		{
			point_lapin++;
			ball_x = 198;
		}
		else
		{
			point_alice++;
			ball_x = 42;
		}
		put_number(point_alice, 2 , 6, 0);
		put_number(point_lapin, 2 , 24 , 0);
	}
}
