import numpy as np
import random
# Define the game matrix
payoffs = np.array([[[2,2], [1,0]], [[0,1], [3,3]]])

# 初始化玩家1和玩家2的策略
for i in range(1,5):
    if i==1:
        player1_strategy = 0  # 0表示r1，1表示r2
        player2_strategy = 0  # 0表示c1，1表示c2
    elif i==2:
        player1_strategy = 0  # 0表示r1，1表示r2
        player2_strategy = 1  # 0表示c1，1表示c2
    elif i==3:
        player1_strategy = 1  # 0表示r1，1表示r2
        player2_strategy = 0  # 0表示c1，1表示c2
    else:  
        player1_strategy = 1  # 0表示r1，1表示r2
        player2_strategy = 1  # 0表示c1，1表示c2
        
    player1_choices = []
    player2_choices = []
    
    # Number of players and strategies
    num_rounds = 100
    
    for round in range(num_rounds):
        
        player1_choices.append(player1_strategy)
        player2_choices.append(player2_strategy)
        
        player2_avg_payoffs = np.mean(player2_choices)
        #print(player2_avg_payoffs)
        if player2_avg_payoffs<0.5:
            if payoffs[0][0][0]>payoffs[1][0][0]:
                player1_strategy = 0
            elif payoffs[0][0][0]<payoffs[1][0][0]:
                player1_strategy = 1
            else:
                player1_strategy = random.randint(0,1)
        elif player2_avg_payoffs>0.5:
            if payoffs[0][1][0]>payoffs[1][1][0]:
                player1_strategy = 0
            elif payoffs[0][1][0]<payoffs[1][1][0]:
                player1_strategy = 1
            else:
                player1_strategy = random.randint(0,1)
        else:
            player1_strategy = random.randint(0,1)
            
        player1_avg_payoffs = np.mean(player1_choices)
        
        if player1_avg_payoffs<0.5:
            if payoffs[0][0][1]>payoffs[0][1][1]:
                player2_strategy = 0
            elif payoffs[0][0][1]<payoffs[0][1][1]:   
                player2_strategy = 1
            else:
                player2_strategy = random.randint(0,1) 
        elif(player1_avg_payoffs>0.5):
            if payoffs[1][0][1]>payoffs[1][1][1]:
                player2_strategy = 0
            elif payoffs[1][0][1]<payoffs[1][1][1]:
                player2_strategy = 1
            else:
                player2_strategy = random.randint(0,1) 
        else:
            player2_strategy = random.randint(0,1)    
           
    if [player1_choices[num_rounds-1],player2_choices[num_rounds-1]] == [1,1]:
        print("converge to (c2,r2)")
    elif [player1_choices[num_rounds-1],player2_choices[num_rounds-1]] == [0,0]:
        print("converge to (c1,r1)")
    else:
        print("not converge")        