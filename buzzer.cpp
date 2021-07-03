#include "buzzer.h" 
#include "periphery.h" 
#include "notes.h" 

SoundBuzzer Buzzer;

// This array contain precalculated values for PWM tim to generate note frequency 
// DO			DO#			RE			RE#			MI			FA			FA#			SOL			SOL#		LA			LA#			SI
uint16_t tone[] = {
	30581,	28860,	27248,	25707,	24272,	22910,	21622,	20408,	19259,	18182,	17160,	16194,
	15291,	14430,	13619,	12858,	12136,	11455,	10811,	10204,	9632,		9091,		8580,		8099,
	7645,		7215,		6811,		6428,		6067,		5727,		5405,		5102,		4816,		4545,		4290,		4049,
	3822,		3608,		3405,		3214,		3034,		2863,		2703,		2551,		2408,		2273,		2145,		2025
};

/*! -------------------------------------------------------------------------
\brief Return current state of the buzzer
*/
uint8_t SoundBuzzer::GetBuzzerState()
{
	return buzzer_state;
}

/*! -------------------------------------------------------------------------
\brief Play track Ode to Joy
\details   This function set TIM PWM with selected note frequency and TIM 
with note duration. TIM PWM frequency MUST be set with 4 MHz input clock.
*/
void SoundBuzzer::PlayMusic(const uint8_t *music, const uint8_t *duration, uint8_t tempo, const uint8_t size)
{
	current_music = music;
	current_duration = duration;
	play_note_time = 600000/tempo;					// 60 sec * 1000 = 60000 ms, and *10 because of TIM prescaler
	silent_note_time = play_note_time/10;
	current_play_size = size;
	buzzer_state = BUZZER_PLAY;
	note_counter = 0;
	PlayNote();
}

/*! -------------------------------------------------------------------------
\brief Play note of the music song
\details Checks note duration and state of the buzzer. Sets timers according
to calculated values.
\param[out] 0 - all fine, 1 - error
*/
bool SoundBuzzer::PlayNote()
{
	if(buzzer_state == BUZZER_IDLE)
	{
		StopMusic();
		return 1;
	}
	
	bool pause = false;
	uint16_t current_play_note_time = play_note_time;
	uint8_t duration = current_duration[note_counter];
	
	if(duration > T_1)		// Note duration and pause duration differ only by flag "pause"
	{
		pause = true;
		duration -= T_1;
	}
	switch(duration)
	{
		case 16: 	current_play_note_time *= 4;																break;		// T_1
		case 15:	current_play_note_time = (current_play_note_time * 15)/4;		break;		// At first multiplication
		case 14:	current_play_note_time = (current_play_note_time * 14)/4;		break;
		case 13:	current_play_note_time = (current_play_note_time * 13)/4;		break;	
		case 12:	current_play_note_time *= 3;																break;	
		case 11:	current_play_note_time = (current_play_note_time * 11)/4;		break;	
		case 10:	current_play_note_time = (current_play_note_time * 10)/4;		break;	
		case 9:		current_play_note_time = (current_play_note_time * 9)/4;		break;	
		case 8: 	current_play_note_time *= 2;																break;		// T_1_2
		case 7:		current_play_note_time = (current_play_note_time * 7)/4;		break;	
		case 6:		current_play_note_time = (current_play_note_time * 6)/4;		break;	
		case 5:		current_play_note_time = (current_play_note_time * 5)/4;		break;	
		case 4:																																break;		// T_1_4 (do nothing)
		case 3:		current_play_note_time = (current_play_note_time * 3)/4;		break;	
		case 2:		current_play_note_time /= 2;																break;		// T_1_8
		case 1:		current_play_note_time /= 4;																break;		// T_1_16
	}

	switch(buzzer_state)
	{
		case BUZZER_PLAY:
			if(!pause) {	SetTimers(tone[current_music[note_counter]], current_play_note_time - silent_note_time); }
			else {	SetTimers(0, current_play_note_time - silent_note_time); }
			buzzer_state = BUZZER_PAUSE;
			break;
		
		case BUZZER_PAUSE:
			SetTimers(0, silent_note_time);
			buzzer_state = BUZZER_PLAY;
			note_counter++;
			if(note_counter >= current_play_size)
				buzzer_state = BUZZER_IDLE;
			break;
	}
	return 0;
}

/*! -------------------------------------------------------------------------
\brief Set frequency and duration timers to play note
\details   
*/
void SoundBuzzer::SetTimers(uint16_t note_frequency, uint16_t note_duration)
{
	if(note_frequency == 0)
	{
		Phy.StopTimPWM();
	}
	else
	{
		Phy.UpdateTimPWM(note_frequency, note_frequency/2);
	}
	
	Phy.UpdateTIM16(note_duration);
}

/*! -------------------------------------------------------------------------
\brief Set frequency and duration timers to play note
\details   
*/
void SoundBuzzer::StopMusic()
{
	Phy.StopTimPWM();
	Phy.StopTIM16();
	buzzer_state = BUZZER_IDLE;
}
