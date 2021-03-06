#include <vector>


#include "../headers/environments/frozen_lake/environment.h"
#include "../headers/algorithms/frozen_lake/q_learning_frozen_lake.h"
#include "../headers/utils/utils.h"


void learn()
{
    FrozenLake frozenLake(false, false);

    int nStates = frozenLake.env.size() * frozenLake.env[0].size();
    std::vector<int> inputShape = {nStates, (int) frozenLake.env[0].size()};
    std::vector<std::vector<double>> qTable = initializeQTable(inputShape);

    int maxStepsPerEpisode = 100;
    int nEpisodes = 10000;
    double learningRate = 0.01;

    double gamma = 1.0;
    double gammaMax = 1.0;
    double gammaMin = 0.01;
    double gammaDecay = 0.001;

    int nextState;
    double reward;
    bool done;

    double total_rewards = 0;

    for (int episode = 0; episode < nEpisodes; episode++)
    {
        double averageReward = 0;
        int state = frozenLake.currentPositionOnGrid;

        for (int step = 0; step < maxStepsPerEpisode; step++)
        {
            int action = chooseAction(qTable[state], 0.5);
            std::tie(nextState, reward, done) = frozenLake.step(action);
            auto maxValue = std::max_element(std::begin(qTable[nextState]), std::end(qTable[nextState]));
            qTable[state][action] = qTable[state][action] + learningRate * (reward + gamma * *maxValue - qTable[state][action]);
            averageReward += reward;

            if (done)
            {
                break;
            }

            state = nextState;

            if (state == frozenLake.finalStateID)
            {
                print("CONGRATULATIONS! You have reached the goal!");
                std::cout << "Episode " << episode << " finished after " << step << " steps" << std::endl;
                break;
            }
        }

        total_rewards += averageReward;

        gamma = updateExplorationRate(gammaMin,
                                      gammaMax,
                                      gammaDecay,
                                      episode);

        if (episode % 100 == 0)
        {
            print("Episode: " + std::to_string(episode + 1) + " finished");
            print("Average reward: " + std::to_string(total_rewards / 100));
            std::cout << std::endl;
            total_rewards = 0;
        }

        frozenLake.reset();

    }
}


int main()
{
    learn();

    return 0;
}
