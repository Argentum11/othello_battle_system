import random
import subprocess
import numpy as np

# strategies
alpha_beta_pruning = 1
monte_carlo = 2
mix = 3
strategy_dict = {
  alpha_beta_pruning: "alpha beta pruning", 
  monte_carlo: "monte carlo", 
  mix: "mix"
}
def get_strategy_exe_file(strategy:int):
    if strategy==alpha_beta_pruning:
        return "ab.exe"
    elif strategy==monte_carlo:
        return "mc.exe"
    else:
        return "mix.exe"

# Let the user decide on strategies and depth/iterations
select_strategy = []
depth_intertion_list = []
order_str = ""
for i in range(2):

    # strategies
    if i==0: order_str = "first"
    else: order_str = "second"
    print(f'\nPlease select {order_str} strategy(',end="")
    for key, strategy_name in strategy_dict.items():
        print(f'{strategy_name}...{key}/',end="")
    print("): ", end="")
    strategy = input()
    select_strategy.append(int(strategy))

    # depth/iterations
    print(f'Please select {order_str} strategy depth/iteration: ',end="")
    depth_iteration = input()
    depth_intertion_list.append(int(depth_iteration))
print("")

# Color
EMPTY = "-"
WHITE = "W"
BLACK = "B"
color_str = ["Black", "White"]
def get_color_number(color:str):
    if color==BLACK:
        return 1
    else:
        return 2

# Decide which strategy goes first
second_strategy_goes_first = random.choice([True, False])
if(second_strategy_goes_first):
    # swap strategy
    temp = select_strategy[0]
    select_strategy[0] = select_strategy[1]
    select_strategy[1] = temp
    # swap depth/iterations
    temp = depth_intertion_list[0]
    depth_intertion_list[0] = depth_intertion_list[1]
    depth_intertion_list[1] = temp

# display strategy color
print(f'{strategy_dict[select_strategy[0]]}({depth_intertion_list[0]}): Black')
print(f'{strategy_dict[select_strategy[1]]}({depth_intertion_list[1]}): White',end="\n\n")

def board_to_str(board:np.array):
    board_string = ""
    for i in range(SIZE):
        for j in range(SIZE):
            board_string = f'{board_string}{board[i][j]}'
    return board_string

def str_to_board(string:str):
    board =  np.empty(shape = [SIZE, SIZE], dtype=str)
    for i in range(SIZE*SIZE):
        board[i//SIZE][i%SIZE] = string[i]
    return board

def output_board(string:str):
    for i in range(SIZE):
        for j in range(SIZE):
            print(string[i*SIZE+j],end=" ")
        print("")
    print("")

def winner(board:np.array):
    black_piece = 0
    for i in range(SIZE):
        for j in range(SIZE):
            if(board[i][j] == BLACK):
                black_piece = black_piece + 1
    white_piece = 0
    for i in range(SIZE):
        for j in range(SIZE):
            if(board[i][j] == WHITE):
                white_piece = white_piece + 1
    #print(f'B: {black_piece}, W: {white_piece}')
    if black_piece > white_piece:
        return 0
    else:
        return 1

SIZE = 6

in_sign = "<"
in_file = "input.txt"
out_sign = ">"
out_file = "output.txt"

player_color = BLACK
strategy_index = 0  
for game_times in range(3):
    # Initialize board
    board =  np.empty(shape = [SIZE, SIZE], dtype=str)
    for i in range(SIZE):
        for j in range(SIZE):
            board[i][j] = EMPTY
    board[SIZE // 2 - 1][SIZE // 2 - 1] = WHITE
    board[SIZE // 2][SIZE // 2] = WHITE
    board[SIZE // 2 - 1][SIZE // 2] = BLACK
    board[SIZE // 2][SIZE // 2 - 1] = BLACK

    previous_board_string = ""
    for i in range(SIZE*SIZE):
        previous_board_string = f'{previous_board_string} '
    board_string = board_to_str(board)
    same_count = 0

    print(f'Battle: {game_times}')
    count = 0
    while True:
        count = count + 1
        same = True
        for i in range(SIZE):
            for j in range(SIZE):
                if previous_board_string[i*SIZE+j]!=board_string[i*SIZE+j]:
                    same = False
                    break
        if same == True:
            same_count = same_count + 1
        if same_count == 2:
            break
        # reset same_count for uncontinuous skip
        if count%2==0:
            same_count = 0

        previous_board_string = board_string
        player_color_number = get_color_number(player_color)
        depth_iteration = depth_intertion_list[1-count%2]
        with open(in_file,'w') as my_file:
            my_file.write(f'{player_color_number} {depth_iteration} {board_string}')
        exe_file = get_strategy_exe_file(select_strategy[strategy_index])
        print(board_string)
        subprocess.run([exe_file, in_sign, in_file, out_sign, out_file], shell=True)

        with open(out_file) as my_file:
            board_string = my_file.read()
        board = str_to_board(board_string)
        #print(board_string)

        # switch_player
        if player_color==BLACK:
            player_color=WHITE
        else:
            player_color=BLACK
        if strategy_index==0:
            strategy_index = 1
        else:
            strategy_index = 0

    # print result
    #print(f'color: {player_color}',end='')
    output_board(board_string)
    winner_index =  winner(board)
    winner_strategy = select_strategy[winner_index]
    print(f'{strategy_dict[winner_strategy]}({color_str[winner_index]}, {depth_intertion_list[winner_index]}) wins!\n\n')