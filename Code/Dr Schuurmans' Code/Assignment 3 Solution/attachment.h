#include <iostream>
#include <cfloat>
#include <Eigen/Dense>
#include <Eigen/Sparse>

#ifndef ATTACHMENT_H
#define ATTACHMENT_H

using namespace Eigen;

struct mesh;
struct skeleton;


struct attachment
{
	mesh* objp;
	skeleton* skelp;

	MatrixXf* D;
	MatrixXi* V;
	MatrixXf* S;
	VectorXf* h;
	MatrixXf* C;
	SparseMatrix<int>* A;
	SparseMatrix<int>* L;
	MatrixXf* W;


	// member functions

	attachment(mesh* objref, skeleton* skelref) : objp(objref), skelp(skelref) {}

	void distancesVisibility();
	void connectionValues();
	void adjacencyLaplacian();
	void attachmentWeights(float);
	void glDrawMeshAttach(bool, int);
	void writeVectorXf(VectorXf*, char*);
	void writeMatrixXf(MatrixXf*, char*);
	void writeMatrixXi(MatrixXi*, char*);
	void writeSparseMatrixXi(SparseMatrix<int>*, char*);
};

#endif
