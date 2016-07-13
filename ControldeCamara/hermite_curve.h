#pragma once
#ifndef HERMITE_CURVE_H
#define HERMITE_CURVE_H

#include "math3d.h"


namespace pcpred
{

	// piecewise cubic hermite curve
	class HermiteCurve
	{
	public:

		HermiteCurve(int num_pieces = 1);

		inline int getNumPieces()
		{
			return num_pieces_;
		}

		void setCurveShape(int num_pieces);
		void setControlPoint(int i, const Eigen::Vector3d& x, const Eigen::Vector3d& v);
		void makeStationaryPoint(const Eigen::Vector3d& x);

		int featureSize();
		Eigen::VectorXd toFeature();

		// output encode/decode
		int encodingSize();
		Eigen::VectorXd encode();
		void decode(const Eigen::VectorXd& code);

		Eigen::Vector3d operator () (double t) const;

		HermiteCurve operator + (const HermiteCurve& rhs) const;
		HermiteCurve operator - (const HermiteCurve& rhs) const;
		HermiteCurve operator * (double rhs) const;

		HermiteCurve& operator += (const HermiteCurve& rhs);
		HermiteCurve& operator -= (const HermiteCurve& rhs);
		HermiteCurve& operator *= (double rhs);

	private:

		int num_pieces_;
		Matrix3x3 control_points_;
	};

}


#endif // HERMITE_CURVE_H
