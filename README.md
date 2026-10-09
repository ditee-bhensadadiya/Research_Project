**Reinforcement Learning-Based Autonomous Vehicle Navigation:**
A reinforcement learning-based autonomous vehicle navigation project that compares Q-Learning, PPO, and A2C for obstacle-aware path planning in a simulated grid-world environment, followed by physical validation using an ESP32-based robotic car.

**Project Overview:**
The system trains reinforcement learning agents to navigate a 16×16 grid-world from a fixed start position to a target while avoiding obstacles.
Three reinforcement learning algorithms were evaluated:
- Q-Learning
- Proximal Policy Optimization (PPO)
- Advantage Actor-Critic (A2C)
Each algorithm was trained for 3,000 episodes across 10 different obstacle configurations. Their performance was compared using reward, success rate, training time, and resource utilization.
A trained Q-Learning policy was then deployed on a physical vehicle using an ESP32, demonstrating the learned navigation path in a 4×4 physical grid.

**System Workflow:**
        Grid-World Environment
                 ↓
        State & Action Selection
                 ↓
       ┌─────────┴─────────┐
       ↓         ↓         ↓
   Q-Learning   PPO       A2C
       └─────────┬─────────┘
                 ↓
        Learn Optimal Policy
                 ↓
       Performance Evaluation
                 ↓
       Q-Learning Policy
                 ↓
        Wi-Fi Communication
                 ↓
          ESP32 Vehicle
                 ↓
       Motor Control & Movement

**Environment:**
The simulation uses a custom 16×16 grid-world consisting of:
- 256 possible states
- 4 actions: Up, Down, Left, Right
- Fixed start and goal positions
- 10 obstacle configurations
- Maximum 300 steps per episode
- 3,000 training episodes
Reward Structure
Event	Reward
Reaching goal	+500
Collision with obstacle	-100
Invalid/wall movement	-10
Normal movement	-1


**Algorithms:**
Q-Learning
A value-based reinforcement learning algorithm using an ε-greedy strategy to learn the optimal action for each state.
Parameters:
- Learning rate (α): 0.1
- Discount factor (γ): 0.95
- Initial ε: 1.0
- Minimum ε: 0.01
- ε decay: 0.995
PPO
Proximal Policy Optimization was used as a policy-gradient based approach for stable policy learning.
A2C
Advantage Actor-Critic combines value estimation with policy optimization to improve navigation performance.

**Results:**
The algorithms were evaluated under the same simulation conditions.
Algorithm	Average Reward	Success Rate
A2C	546.06	95.28%
PPO	452.58	90.01%
Q-Learning	302.91	63.73%


**Performance Ranking:**
1. 🥇 A2C — Best overall navigation performance
2. 🥈 PPO — Strong performance and good balance
3. 🥉 Q-Learning — Simple and highly resource-efficient
Q-Learning required significantly fewer computational resources, while A2C achieved the highest reward and success rate.

**Physical Vehicle:**
A proof-of-concept autonomous vehicle was built using:
- ESP32 Microcontroller
- TB6612FNG Motor Driver
- DC Motors
- Wi-Fi Communication
- Vehicle Chassis
- Power Supply
The trained Q-Learning navigation policy was transmitted wirelessly to the ESP32. The physical vehicle was tested on a 4×4 grid, where it successfully followed the learned path and reached the target.
Note: The comparative performance results for Q-Learning, PPO, and A2C are based on the 16×16 simulation. The physical vehicle validation was performed using the Q-Learning policy.

**Tech Stack:**
Programming & ML
- Python
- NumPy
- Pandas
- Matplotlib
- Gymnasium / OpenAI Gym
- Stable-Baselines3
- PyTorch
- Jupyter Notebook
Hardware
- ESP32
- TB6612FNG
- DC Motors
- Wi-Fi

**Evaluation Metrics:**
The project evaluates the reinforcement learning algorithms using:
- Average Reward
- Success Rate
- Average Steps
- Training Time
- CPU Utilization
- RAM Utilization

**Applications:**
The approach can be extended to:
- Autonomous vehicles
- Warehouse robots
- Delivery robots
- Indoor navigation systems
- Intelligent robotic navigation

**Future Scope:**
- Real-time obstacle detection using sensors
- Dynamic obstacle avoidance
- Integration with LiDAR or camera-based perception
- Continuous navigation in real-world environments
- Testing PPO and A2C policies on physical robotic platforms
- ROS-based autonomous navigation

**Team:**
Ditee Bhensadadiya
Krishna Varsani
