#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>



// buzzer states
const uint8_t BUZZER_IDLE = 0;
const uint8_t BUZZER_PLAY = 1;
const uint8_t BUZZER_PAUSE = 2;

class SoundBuzzer
{
	public:
		void PlayMusic(const uint8_t *music, const uint8_t *duration, uint8_t tempo, const uint8_t size);
		bool PlayNote();
		void StopMusic();
	
		uint8_t GetBuzzerState();
	
	private:
		void SetTimers(uint16_t note_frequency, uint16_t note_duration);

		const uint8_t *current_music;
		const uint8_t *current_duration;
		uint16_t play_note_time;
		uint16_t silent_note_time;
		uint8_t current_play_size;
		uint8_t buzzer_state;
		uint16_t note_counter;
};

extern SoundBuzzer Buzzer;


#endif

