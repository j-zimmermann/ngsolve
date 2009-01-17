#ifndef FILE_COMMUTING_AMG
#define FILE_COMMUTING_AMG

/* *************************************************************************/
/* File:   commuting_amg.hoo                                               */
/* Author: Joachim Schoeberl                                               */
/* Date:   15. Aug. 2002                                                   */
/* *************************************************************************/


class CommutingAMG : public BaseMatrix
{
protected:
  const BaseSparseMatrix * pmat;
  CommutingAMG * recAMG;

  SparseMatrixTM<double> * prol;
  
  BaseSparseMatrix * coarsemat;
  BaseJacobiPrecond * jacobi;
  BaseBlockJacobiPrecond * bjacobi;
  BaseMatrix * inv;

public:
  CommutingAMG ()
  { ; }
  virtual ~CommutingAMG () 
  { ; }

  virtual void ComputeMatrices (const BaseSparseMatrix & mat) = 0;
  virtual void Mult (const BaseVector & x, BaseVector & y) const = 0;


  virtual int VHeight() const { return pmat->Height(); }
  virtual int VWidth() const { return pmat->Width(); }

  virtual int NZE() const = 0;
  virtual BaseVector * CreateVector () const
  {
    return pmat->CreateVector();
  }
};


class AMG_H1 : public CommutingAMG
{
public:

  AMG_H1 (const BaseMatrix & sysmat,
	  ARRAY<ngstd::INT<2> > & e2v,
	  ARRAY<double> & weighte,
	  int levels);

  virtual ~AMG_H1 ();

  virtual void ComputeMatrices (const BaseSparseMatrix & mat);
  virtual int NZE() const;

  virtual void Mult (const BaseVector & x, BaseVector & y) const;
};




class AMG_HCurl : public CommutingAMG
{
  SparseMatrixTM<double> * grad;

  BaseSparseMatrix * h1mat;
  AMG_H1 * h1AMG;

public:

  AMG_HCurl (const BaseMatrix & sysmat,
	     const ARRAY<Vec<3> > & vertices,
             ARRAY<ngstd::INT<2> > & e2v,
	     ARRAY<ngstd::INT<4> > & f2v,
	     ARRAY<double> & weighte,
	     ARRAY<double> & weightf,
	     int levels);

  virtual ~AMG_HCurl ();

  virtual void ComputeMatrices (const BaseSparseMatrix & mat);
  virtual int NZE() const;

  virtual void Mult (const BaseVector & x, BaseVector & y) const;
};





#endif
