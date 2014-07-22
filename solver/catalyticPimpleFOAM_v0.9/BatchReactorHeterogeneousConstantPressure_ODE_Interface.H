/*----------------------------------------------------------------------*\
|     ____                    ______ __  __  ____  _  ________            |
|    / __ \                  /  ___ |  \/  |/ __ \| |/ /  ____|           |
|   | |  | |_ __   ___ _ __ |  (___ | \  / | |  | | ' /| |__              |
|   | |  | | '_ \ / _ \ '_ \ \___  \| |\/| | |  | |  < |  __|             |
|   | |__| | |_) |  __/ | | |____)  | |  | | |__| | . \| |____            |
|    \____/| .__/ \___|_| |_|______/|_|  |_|\____/|_|\_\______|           |
|          | |                                                            |
|          |_|                                                            |
|                                                                         |
|   CRECK Modeling Group <http://creckmodeling.chem.polimi.it>            |
|   Department of Chemistry, Materials and Chemical Engineering           |
|   Politecnico di Milano                                                 |
|   Author: Alberto Cuoci <alberto.cuoci@polimi.it>                       |
|	Date: 07 Mar 2013                                                     |
|-------------------------------------------------------------------------|
|	License                                                               |
|                                                                         |
|   This file is part of OpenSMOKE.                                       |
|                                                                         |
|   OpenSMOKE is free software: you can redistribute it and/or modify     |
|   it under the terms of the GNU General Public License as published by  |
|   the Free Software Foundation, either version 3 of the License, or     |
|   (at your option) any later version.                                   |
|                                                                         |
|   OpenSMOKE is distributed in the hope that it will be useful,          |
|   but WITHOUT ANY WARRANTY; without even the implied warranty of        |
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
|   GNU General Public License for more details.                          |
|                                                                         |
|   You should have received a copy of the GNU General Public License     |
|   along with OpenSMOKE. If not, see <http://www.gnu.org/licenses/>.     |
|                                                                         |
\*-----------------------------------------------------------------------*/

#ifndef BatchReactorHeterogeneousConstantPressure_ODE_Interface_H
#define	BatchReactorHeterogeneousConstantPressure_ODE_Interface_H

#include "math/OpenSMOKEVector.h"
#include "ode/OpenSMOKE_OdeSystemObject.h"

#if BZZMATH_SUPPORT == 1
#include "BzzMath.hpp"
#endif

#if DVODE_SUPPORT == 1
#include "ode/OpenSMOKE_DVODE_Interface.h"
#include "ode/OpenSMOKE_DVODE.h"
#endif

#if ODEPACK_SUPPORT == 1
#include "ode/OpenSMOKE_DLSODE_Interface.h"
#include "ode/OpenSMOKE_DLSODE.h"
#include "ode/OpenSMOKE_DLSODA_Interface.h"
#include "ode/OpenSMOKE_DLSODA.h"
#endif

#if DASPK_SUPPORT == 1
#include "ode/OpenSMOKE_DASPK_Interface.h"
#include "ode/OpenSMOKE_DASPK.h"
#endif

#if RADAU_SUPPORT == 1
#include "ode/OpenSMOKE_RADAU_Interface.h"
#include "ode/OpenSMOKE_RADAU.h"
#endif

#if SUNDIALS_SUPPORT == 1
#include "ode/OpenSMOKE_CVODE_Sundials_Interface.h"
#include "ode/OpenSMOKE_CVODE_Sundials.h"
#endif

#if MEBDF_SUPPORT == 1
#include "ode/OpenSMOKE_MEBDF_Interface.h"
#include "ode/OpenSMOKE_MEBDF.h"
#endif

#include "math/stiff-ode-solvers/OdeSystemObject.h"
#include "BatchReactorHeterogeneousConstantPressure.H"

namespace OpenSMOKE
{
	class BatchReactorHeterogeneousConstantPressure_ODE_OpenSMOKE : public UserOdeSystemObject 
	{
	public:

		BatchReactorHeterogeneousConstantPressure_ODE_OpenSMOKE( BatchReactorHeterogeneousConstantPressure& batch ) :
		batch_(batch)
		{
			ChangeDimensions(batch_.NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_.NumberOfEquations(), &dy_, false);
		}
	
		virtual void GetSystemFunctions(const Eigen::VectorXd &Y, const double t, Eigen::VectorXd &DY)
		{
			y_.CopyFrom(Y.data());
			batch_.Equations(t, y_, dy_);
			dy_.CopyTo(DY.data());
		}

		void MyPrint(const Eigen::VectorXd &Y, const double t)
		{
			y_.CopyFrom(Y.data());
			batch_.Print(t, y_);
		}

	private:

		BatchReactorHeterogeneousConstantPressure& batch_;
		OpenSMOKE::OpenSMOKEVectorDouble  y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	};
	
	#if DVODE_SUPPORT == 1
	class BatchReactorHeterogeneousConstantPressure_ODE_DVODE : public OpenSMOKE::OpenSMOKE_OdeSystemObject
	{
		DEFINE_ODESOLVERINTERFACE_DVODE(BatchReactorHeterogeneousConstantPressure_ODE_DVODE)

		BatchReactorHeterogeneousConstantPressure* batch_;
		OpenSMOKE::OpenSMOKEVectorDouble y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	
	public:

		void SetBatchReactorHeterogeneousConstantPressure(BatchReactorHeterogeneousConstantPressure* batch)
		{
			batch_ = batch;
			ChangeDimensions(batch_->NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_->NumberOfEquations(), &dy_, false);
		}
	 
		int GetSystemFunctions(const double t, double* y,  double* dy)
		{
			y_.CopyFrom(y);
			int flag = batch_->Equations(t, y_, dy_);
			dy_.CopyTo(dy);
			return(flag);
		}
	
		int GetAnalyticalJacobian(const double t,  double* y,  double* J)
		{
			return(0);
		}
	 
		int GetWriteFunction(const double t, double *y)
		{
			y_.CopyFrom(y);
			int flag = batch_->Print(t, y_);
			return 0;
 		}
	}; 
	COMPLETE_ODESOLVERINTERFACE_DVODE(BatchReactorHeterogeneousConstantPressure_ODE_DVODE)
	#endif

	#if ODEPACK_SUPPORT == 1
	class BatchReactorHeterogeneousConstantPressure_ODE_DLSODE : public OpenSMOKE::OpenSMOKE_OdeSystemObject
	{
		DEFINE_ODESOLVERINTERFACE_DLSODE(BatchReactorHeterogeneousConstantPressure_ODE_DLSODE)

		BatchReactorHeterogeneousConstantPressure* batch_;
		OpenSMOKE::OpenSMOKEVectorDouble y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	
	public:

		void SetBatchReactorHeterogeneousConstantPressure(BatchReactorHeterogeneousConstantPressure* batch)
		{
			batch_ = batch;
			ChangeDimensions(batch_->NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_->NumberOfEquations(), &dy_, false);
		}
	 
		int GetSystemFunctions(const double t, double* y,  double* dy)
		{
			y_.CopyFrom(y);
			int flag = batch_->Equations(t, y_, dy_);
			dy_.CopyTo(dy);
			return(flag);
		}
	
		int GetAnalyticalJacobian(const double t,  double* y,  double* J)
		{
			return(0);
		}
	 
		int GetWriteFunction(const double t, double *y)
		{
			y_.CopyFrom(y);
			int flag = batch_->Print(t, y_);
			return 0;
 		}
	}; 
	COMPLETE_ODESOLVERINTERFACE_DLSODE(BatchReactorHeterogeneousConstantPressure_ODE_DLSODE)

	class BatchReactorHeterogeneousConstantPressure_ODE_DLSODA : public OpenSMOKE::OpenSMOKE_OdeSystemObject
	{
		DEFINE_ODESOLVERINTERFACE_DLSODA(BatchReactorHeterogeneousConstantPressure_ODE_DLSODA)

		BatchReactorHeterogeneousConstantPressure* batch_;
		OpenSMOKE::OpenSMOKEVectorDouble y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	
	public:

		void SetBatchReactorHeterogeneousConstantPressure(BatchReactorHeterogeneousConstantPressure* batch)
		{
			batch_ = batch;
			ChangeDimensions(batch_->NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_->NumberOfEquations(), &dy_, false);
		}
	 
		int GetSystemFunctions(const double t, double* y,  double* dy)
		{
			y_.CopyFrom(y);
			int flag = batch_->Equations(t, y_, dy_);
			dy_.CopyTo(dy);
			return(flag);
		}
	
		int GetAnalyticalJacobian(const double t,  double* y,  double* J)
		{
			return(0);
		}
	 
		int GetWriteFunction(const double t, double *y)
		{
			y_.CopyFrom(y);
			int flag = batch_->Print(t, y_);
			return 0;
 		}
	}; 
	COMPLETE_ODESOLVERINTERFACE_DLSODA(BatchReactorHeterogeneousConstantPressure_ODE_DLSODA)
	#endif

	#if DASPK_SUPPORT == 1
	class BatchReactorHeterogeneousConstantPressure_ODE_DASPK : public OpenSMOKE::OpenSMOKE_OdeSystemObject
	{
		BatchReactorHeterogeneousConstantPressure* batch_;

		DEFINE_ODESOLVERINTERFACE_DASPK(BatchReactorHeterogeneousConstantPressure_ODE_DASPK)

		OpenSMOKE::OpenSMOKEVectorDouble y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	
	public:

		void SetBatchReactorHeterogeneousConstantPressure(BatchReactorHeterogeneousConstantPressure* batch)
		{
			batch_ = batch;
			ChangeDimensions(batch_->NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_->NumberOfEquations(), &dy_, false);
		}
	 
		int GetSystemFunctions(const double t, double* y,  double* dy)
		{
			y_.CopyFrom(y);
			int flag = batch_->Equations(t, y_, dy_);
			dy_.CopyTo(dy);
			return(flag);
		}
	
		int GetAnalyticalJacobian(const double t,  double* y,  double* J)
		{
			return(0);
		}
	 
		int GetWriteFunction(const double t, double *y)
		{
			y_.CopyFrom(y);
			int flag = batch_->Print(t, y_);
			return 0;
 		}
	}; 
	COMPLETE_ODESOLVERINTERFACE_DASPK(BatchReactorHeterogeneousConstantPressure_ODE_DASPK)
	#endif

	#if RADAU_SUPPORT == 1
	class BatchReactorHeterogeneousConstantPressure_ODE_RADAU5 : public OpenSMOKE::OpenSMOKE_OdeSystemObject
	{
		DEFINE_ODESOLVERINTERFACE_RADAU(BatchReactorHeterogeneousConstantPressure_ODE_RADAU5)

		BatchReactorHeterogeneousConstantPressure* batch_;
		OpenSMOKE::OpenSMOKEVectorDouble y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	
	public:

		void SetBatchReactorHeterogeneousConstantPressure(BatchReactorHeterogeneousConstantPressure* batch)
		{
			batch_ = batch;
			ChangeDimensions(batch_->NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_->NumberOfEquations(), &dy_, false);
		}
	 
		int GetSystemFunctions(const double t, double* y,  double* dy)
		{
			y_.CopyFrom(y);
			int flag = batch_->Equations(t, y_, dy_);
			dy_.CopyTo(dy);
			return(flag);
		}
	
		int GetAnalyticalJacobian(const double t,  double* y,  double* J)
		{
			return(0);
		}
	 
		int GetWriteFunction(const double t, double *y)
		{
			y_.CopyFrom(y);
			int flag = batch_->Print(t, y_);
			return 0;
 		}
	}; 
	COMPLETE_ODESOLVERINTERFACE_RADAU(BatchReactorHeterogeneousConstantPressure_ODE_RADAU5)
	#endif

	#if SUNDIALS_SUPPORT == 1
	class BatchReactorHeterogeneousConstantPressure_ODE_CVODE : public OpenSMOKE::OpenSMOKE_OdeSystemObject
	{
		DEFINE_ODESOLVERINTERFACE_CVODE_Sundials(BatchReactorHeterogeneousConstantPressure_ODE_CVODE)

		BatchReactorHeterogeneousConstantPressure* batch_;
		OpenSMOKE::OpenSMOKEVectorDouble y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	
	public:

		void SetBatchReactorHeterogeneousConstantPressure(BatchReactorHeterogeneousConstantPressure* batch)
		{
			batch_ = batch;
			ChangeDimensions(batch_->NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_->NumberOfEquations(), &dy_, false);
		}
	 
		int GetSystemFunctions(const double t, double* y,  double* dy)
		{
			y_.CopyFrom(y);
			int flag = batch_->Equations(t, y_, dy_);
			dy_.CopyTo(dy);
			return(flag);
		}
	
		int GetAnalyticalJacobian(const double t,  double* y,  double* J)
		{
			return(0);
		}
	 
		int GetWriteFunction(const double t, double *y)
		{
			y_.CopyFrom(y);
			int flag = batch_->Print(t, y_);
			return 0;
 		}
	}; 
	COMPLETE_ODESOLVERINTERFACE_CVODE_Sundials(BatchReactorHeterogeneousConstantPressure_ODE_CVODE)
	#endif

	#if MEBDF_SUPPORT == 1
	class BatchReactorHeterogeneousConstantPressure_ODE_MEBDF : public OpenSMOKE::OpenSMOKE_OdeSystemObject
	{
		DEFINE_ODESOLVERINTERFACE_MEBDF(BatchReactorHeterogeneousConstantPressure_ODE_MEBDF)

		BatchReactorHeterogeneousConstantPressure* batch_;
		OpenSMOKE::OpenSMOKEVectorDouble y_;
		OpenSMOKE::OpenSMOKEVectorDouble dy_;
	
	public:

		void SetBatchReactorHeterogeneousConstantPressure(BatchReactorHeterogeneousConstantPressure* batch)
		{
			batch_ = batch;
			ChangeDimensions(batch_->NumberOfEquations(), &y_, true);
			ChangeDimensions(batch_->NumberOfEquations(), &dy_, false);
		}
	 
		int GetSystemFunctions(const double t, double* y,  double* dy)
		{
			y_.CopyFrom(y);
			int flag = batch_->Equations(t, y_, dy_);
			dy_.CopyTo(dy);
			return(flag);
		}
	
		int GetAnalyticalJacobian(const double t,  double* y,  double* J)
		{
			return(0);
		}
	 
		int GetWriteFunction(const double t, double *y)
		{
			y_.CopyFrom(y);
			int flag = batch_->Print(t, y_);
			return 0;
 		}
	}; 
	COMPLETE_ODESOLVERINTERFACE_MEBDF(BatchReactorHeterogeneousConstantPressure_ODE_MEBDF)
	#endif
}

#endif	// BatchReactorHeterogeneousConstantPressure_ODE_Interface_H
