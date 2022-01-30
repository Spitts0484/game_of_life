class Board{
    public:
        // Public methods
        Board(int);
        ~Board();
        void print();

    private:
        // Private methods
        void init_board();
        void calc_next_board();
        void run();

    public:
        // Public attributes

    private:
        // Private attributes
        int time_steps;
        int board_n_rows = 55;
        int board_n_cols = 211;
        int current_board = 0;
        int cells[2][55][211];
};
