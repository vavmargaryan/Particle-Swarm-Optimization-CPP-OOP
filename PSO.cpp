#include "PSO.h"

// <Particle>

Particle::Particle(const int dim, const std::vector<double>& min, const std::vector<double>& max)
			: pos(dim)
			, vel(dim)
			, bpos(dim)
			, bval(HUGE_VAL)
{
			// generating particle with random position in defined space [min, max]
			for (size_t i = 0; i < dim; ++i) {
						pos[i] = uniformxy(min[i], max[i]);
			}

			// initial velocity is set to [0, 0, ..., 0]
			std::fill(vel.begin(), vel.end(), 0);

			// personal best position is set to initial position
			std::copy(pos.begin(), pos.end(), bpos.begin());
}

// </Particle>

// <Swarm>

Swarm::Swarm(const std::vector<double>& min, const std::vector<double>& max, const int nParticles)
			: nParticles_(nParticles)
			, nDim_(min.size())
			, gbval_(HUGE_VAL)
			, gbpos_(nDim_, HUGE_VAL)
			, min_(min)
			, max_(max)
{
			swarm_.reserve(nParticles_);
			for (int i = 0; i < nParticles_; ++i) {
						swarm_.emplace_back(nDim_, min, max);
			}
}

void Swarm::evaluate(const std::function<double(const std::vector<double>&)>& fn, std::vector<double>& vals)
{
			for (size_t i = 0; i < swarm_.size(); ++i) {
						// evaluate
						vals[i] = fn(swarm_[i].pos);
						// update
						if (vals[i] < swarm_[i].bval) {
									swarm_[i].bpos = swarm_[i].pos;
									swarm_[i].bval = vals[i];
						}

						if (swarm_[i].bval < gbval_) {
									gbval_ = swarm_[i].bval;
									gbpos_ = swarm_[i].bpos;
						}
			}
}

void Swarm::migrate(const Parameters& params)
{
			const double& omega = params.omega;
			const double& phip = params.phip;
			const double& phig = params.phig;

			std::vector<std::vector<double>> pos(nParticles_);
			for (size_t i = 0; i < pos.size(); ++i) {
						pos[i].resize(nDim_);
			}

			std::vector<std::vector<double>> vel(nParticles_);
			for (size_t i = 0; i < vel.size(); ++i) {
						vel[i].resize(nDim_);
			}

			// global coeff
			const double rg = uniform01();

			for (int i = 0; i < nParticles_; ++i) {
						// personal coeff
						const double rp = uniform01();
						bool ok = true;

						Particle& p = swarm_[i];
						std::fill(vel[i].begin(), vel[i].end(), 0);
						std::fill(pos[i].begin(), pos[i].end(), 0);
						for (int j = 0; j < nDim_; ++j) {
									vel[i][j] = omega * p.vel[j] +
												phip * rp * (p.bpos[j] - p.pos[j]) +
												phig * rg * (gbpos_[j] - p.pos[j]);
									pos[i][j] = p.pos[j] + vel[i][j];
									ok = ok && min_[j] < pos[i][j] && pos[i][j] < max_[j];
						}
						if (!ok) {
									for (int j = 0; j < nDim_; ++j) {
												pos[i][j] = min_[j] + (max_[j] - min_[j]) * uniform01();
									}
						}
						p.pos = pos[i];
						p.vel = vel[i];
			}
}


// </Swarm>

// <State>

State::State(const std::vector<double>& min, const std::vector<double>& max, const int nParticles)
			: iter(0)
			, swarm(min, max, nParticles)
{
}

void State::evaluate(const std::function<double(const std::vector<double>&)>& fn, const Parameters& params)
{
			std::vector<double> vals(swarm.getNumberOfParticles());
			swarm.evaluate(fn, vals);
			swarm.migrate(params);
			++iter;
}

// </State>

// <PSO>

PSO::PSO(const std::vector<double>& min, const std::vector<double>& max, const Parameters& params, const int nParticles)
			: min_(min)
			, max_(max)
			, params_(params)
			, state_(min, max, nParticles)
{
}

void PSO::iterate(const std::function<double(const std::vector<double>&)>& fn, const int nIter)
{
			if (nIter == INT32_MAX) {
						State old(state_);
						while (true) {
									optimize(fn);
									if (state_ == old) {
												break;
									}
									old = state_;
						}
			}
			else {
						for (int i = 0; i < nIter; ++i) {
									optimize(fn);
						}
			}
}

void PSO::optimize(const std::function<double(const std::vector<double>&)>& fn)
{
			state_.evaluate(fn, params_);
}


// </PSO>
