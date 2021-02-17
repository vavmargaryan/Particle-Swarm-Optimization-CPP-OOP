#pragma once
#include <iostream>
#include <functional>
#include <vector>

#include "utils.h"

struct Parameters
{
			double omega; // intertia weight
			double phip; // phi personal or c_1
			double phig; // phi global or c_2

			bool operator==(const Parameters& rhs)
			{
						return omega == rhs.omega
									&& phip == rhs.phip
									&& phig == rhs.phig;
			}
};

class Particle
{
public:
			Particle(const int dim, const std::vector<double>& min, const std::vector<double>& max);

			bool operator==(const Particle& rhs) const
			{
						return vector_equals(pos, rhs.pos)
									&& vector_equals(vel, rhs.vel)
									&& vector_equals(bpos, rhs.bpos)
									&& double_equals(bval, rhs.bval);
			}

public:
			std::vector<double> pos;		// position
			std::vector<double> vel;		// velocity
			
			std::vector<double> bpos; // porsonal best position
			double bval;														// personal best value

};

class Swarm
{
public:
			Swarm(const std::vector<double>& min, const std::vector<double>& max, const int nParticles);

			const int getNumberOfParticles() const { return nParticles_; }
			const double getGlobalBestValue() const { return gbval_; }
			const std::vector<double>& getGlobalBestPosition() const { return gbpos_; }

			bool operator==(const Swarm& rhs)
			{
						return gbval_ == rhs.gbval_
									&& nParticles_ == rhs.nParticles_
									&& nDim_ == rhs.nDim_
									&& vector_equals(gbpos_, rhs.gbpos_)
									&& vector_equals(swarm_, rhs.swarm_);
			}

			void evaluate(const std::function<double(const std::vector<double>&)>& fn, std::vector<double>& vals);
			void migrate(const Parameters& params);

private:
			int nParticles_;											// number of particles
			int nDim_;																	// dimension

			double gbval_;																// global best value
			std::vector<double> gbpos_;			// global best position
			std::vector<Particle> swarm_;	// swarm of particles

			std::vector<double> min_; // minimum acceptable values for each dimension
			std::vector<double> max_; // maximum acceptable values for each dimension

};

class State
{
public:
			State(const std::vector<double>& min, const std::vector<double>& max, const int nParticles);

			bool operator==(const State& rhs)
			{
						return swarm == rhs.swarm;
			}

			void evaluate(const std::function<double(const std::vector<double>&)>& fn, const Parameters& params);

			void report(const std::string& testFunc) const
			{
						std::cout << "Test Function        : " << testFunc << '\n';
						std::cout << "Iterations           : " << iter << '\n';
						std::cout << "Global Best Position : " << swarm.getGlobalBestPosition() << '\n';
						std::cout << "Global Best Value    : " << swarm.getGlobalBestValue() << '\n';
			}

public:
			int iter;
			Swarm swarm;

};

class PSO
{
public:
			PSO(const std::vector<double>& min, const std::vector<double>& max, const Parameters& params, const int nParticles);

			void iterate(const std::function<double(const std::vector<double>&)>& fn, const int nIter);

			const State& getState() const { return state_; }

private:
			void optimize(const std::function<double(const std::vector<double>&)>& fn);

private:
			std::vector<double> min_; // minimum acceptable values for each dimension
			std::vector<double> max_; // maximum acceptable values for each dimension

			Parameters params_;
			State state_;

};