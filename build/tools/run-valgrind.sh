cd Debug_x64/
valgrind --log-file=$HOME/leaks.log --smc-check=all --leak-check=full ./player3
