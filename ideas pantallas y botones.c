typedef struct screen_t screen_t;
typedef struct buttons_t buttons_t;

struct screen_t{
	button_t * buttons;
	uint8_t NumberOfButtons;
	ALLEGRO_COLOR BackColor;
};

struct button_t {
	bitmap comun
	bitmap hover
	bitmap clicked
	bool_t clicked
	uint16_t x
	uint16_t y
};