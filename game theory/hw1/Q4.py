import numpy as np

# Game matrix
payoff_matrix = np.array([[[0, 1], [2, 0]], [[2, 0], [0, 4]]])

# Number of iterations
num_iterations = 10000

# Initialize players' strategies
player1_strategy = np.array([0.5, 0.5])
player2_strategy = np.array([0.5, 0.5])

# Lists to track the evolution of strategies
player1_strategy_history = [player1_strategy.copy()]
player2_strategy_history = [player2_strategy.copy()]

# Perform fictitious play iterations
for _ in range(num_iterations):
    # Player 1's best response to player 2's strategy
    best_response1 = np.argmax(np.sum(payoff_matrix * player2_strategy, axis=1))
    player1_strategy = np.zeros(2)
    player1_strategy[best_response1] = 1

    # Player 2's best response to player 1's strategy
    best_response2 = np.argmax(np.sum(payoff_matrix * player1_strategy[:, np.newaxis], axis=0))
    player2_strategy = np.zeros(2)
    player2_strategy[best_response2] = 1

    player1_strategy_history.append(player1_strategy.copy())
    player2_strategy_history.append(player2_strategy.copy())

# Print the final strategies
print("Player 1's final strategy:", player1_strategy)
print("Player 2's final strategy:", player2_strategy)

# Check if the strategies converge to the mixed-strategy Nash equilibrium
epsilon = 1e-4  # A small threshold for convergence
if np.all(np.isclose(player1_strategy, [4/5, 1/5], atol=epsilon)) and np.all(np.isclose(player2_strategy, [0.5, 0.5], atol=epsilon)):
    print("Fictitious play converged to the mixed-strategy Nash equilibrium.")
else:
    print("Fictitious play did not converge to the mixed-strategy Nash equilibrium.")
