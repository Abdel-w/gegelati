#ifndef FAKE_LEARNING_ENVIRONMENT_H
#define FAKE_LEARNING_ENVIRONMENT_H

#include <vector>
#include "learn/learningEnvironment.h"

namespace Learn {

    /**
     * \brief FakeLearningEnvironment class for testing purposes.
     *
     * This class simulates a fake learning environment for testing and debugging.
     * It returns fake data in the form of a FIFO implementation with pre-known numbers.
     */
    class FakeLearningEnvironment : public LearningEnvironment {
      private:
        /// Fake data in the form of a FIFO queue.
        std::vector<int> fakeData;
        /// Current index for retrieving data from the FIFO queue.
        size_t currentIndex;

      public:
        /**
         * \brief Constructor for FakeLearningEnvironment.
         *
         * \param[in] nbActions Number of actions available for interacting with this FakeLearningEnvironment.
         * \param[in] fakeData Fake data in the form of a FIFO queue.
         */
        FakeLearningEnvironment(uint64_t nbActions, const std::vector<int>& fakeData)
            : LearningEnvironment(nbActions), fakeData(fakeData), currentIndex(0) {}

        /**
         * \brief Implementation of the reset method for FakeLearningEnvironment.
         *
         * \param[in] seed The integer value for controlling the randomness of the FakeLearningEnvironment.
         * \param[in] mode LearningMode in which the FakeLearningEnvironment should be reset.
         * \param[in] iterationNumber The integer value to indicate the current iteration number.
         * \param[in] generationNumber The integer value to indicate the current generation number.
         */
        void reset(size_t seed = 0, LearningMode mode = LearningMode::TRAINING, uint16_t iterationNumber = 0, uint64_t generationNumber = 0) override {
            // Reset currentIndex to the beginning of the fakeData FIFO queue.
            currentIndex = 0;
        }

        /**
         * \brief Implementation of the doAction method for FakeLearningEnvironment.
         *
         * \param[in] actionID The integer number representing the action to execute.
         * \throw std::runtime_error if the actionID exceeds nbActions - 1.
         */
        void doAction(uint64_t actionID) override {
            // Perform the action (not relevant for this fake environment).
            // In a real environment, this is where the state would be updated based on the action.
            // Here, we just increment the currentIndex to simulate data retrieval.
            currentIndex++;
        }

        /**
         * \brief Implementation of the getDataSources method for FakeLearningEnvironment.
         *
         * \return A vector of references to the fake data sources.
         */
        std::vector<std::reference_wrapper<const Data::DataHandler>> getDataSources() override {
            // In a real environment, this is where actual data sources would be returned.
            // Here, we return a reference to the fake data source.
            //return {fakeData};
        }

        /**
         * \brief Implementation of the getScore method for FakeLearningEnvironment.
         *
         * \return The current score for the FakeLearningEnvironment.
         */
        double getScore() const override {
            // In a real environment, this is where the current score would be calculated.
            // Here, we return a fixed score for simplicity.
            return 0.0;
        }

        /**
         * \brief Implementation of the isTerminal method for FakeLearningEnvironment.
         *
         * \return A boolean indicating whether the FakeLearningEnvironment has reached a terminal state.
         */
        bool isTerminal() const override {
            // In a real environment, this is where the terminal state would be checked.
            // Here, we return false for simplicity.
            return false;
        }
    };
}; // namespace Learn

#endif
