#ifndef GAME_STATUS_HPP
#define GAME_STATUS_HPP

enum GameStopReason {
    NOT_STOPPING = -1,
    PREPARING = 0,
    PAUSED,
    LOSED,
    WON
};
enum GameStatus {
    STOP = 0,
    RUNNING = 1
};
enum GameRunStatus {
    NOT_REFRESHING,
    REFRESHING
};

#endif // GAME_STATUS_HPP