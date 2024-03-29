#pragma once
#include <cstdint>

enum EventType {
	ADD_POINTS,
	PLAYER_DIED,
	GAME_WIN,
	GAME_OVER,
	START_GAME
};

//template<typename... EventArg>
//struct Event {
//	static const uint8_t MAX_ARGS = 8;
//
//	EventType m_type;
//	uint8_t m_numArgs;
//	EventArg m_args[MAX_ARGS];
//
//	Event(EventType type, uint8_t numArgs, const EventArg&... args)
//		: m_type{ type }
//		, m_numArgs{ numArgs }
//		, m_args{ args }
//	{}
//};