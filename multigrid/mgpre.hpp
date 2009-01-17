#ifndef FILE_MGPRE
#define FILE_MGPRE

/*********************************************************************/
/* File:   mgpre.hh                                                  */
/* Author: Joachim Schoeberl                                         */
/* Date:   20. Apr. 2000                                             */
/*********************************************************************/

/** 
   Multigrid preconditioner
*/

class Smoother;
///
class Prolongation;


///
class MultigridPreconditioner : public BaseMatrix
{

public:
  ///
  enum COARSETYPE { EXACT_COARSE, CG_COARSE, SMOOTHING_COARSE, USER_COARSE };

private:
  ///
  const MeshAccess & ma;
  ///
  const FESpace & fespace;
  ///
  const BilinearForm & biform;
  
  ///
  Smoother * smoother;
  ///
  Prolongation * prolongation;
  ///
  BaseMatrix * coarsegridpre;
  ///
  double checksumcgpre;
  ///
  int ownsmoother, ownprolongation, owncoarsegridpre;
		    
  ///
  COARSETYPE coarsetype;
  ///
  int cycle, incsmooth, smoothingsteps;
  ///
  int coarsesmoothingsteps;
  ///
  int updateall;
  /// creates a new smoother for each update
  bool update_always; 
  /// for robust prolongation
  ARRAY<BaseMatrix*> prol_projection;
public:
  ///
  MultigridPreconditioner (const MeshAccess & ama,
			   const FESpace & afespace,
			   const BilinearForm & abiform,
			   Smoother * asmoother,
			   Prolongation * aprolongation);
  ///
  ~MultigridPreconditioner ();

  ///
  void FreeMem(void);

  ///
  void SetSmoothingSteps (int sstep);
  ///
  void SetCycle (int c);
  ///
  void SetIncreaseSmoothingSteps (int incsm);
  ///
  void SetCoarseType (COARSETYPE ctyp);
  ///
  void SetCoarseGridPreconditioner (const BaseMatrix * acoarsegridpre);
  ///
  void SetCoarseSmoothingSteps (int cstep);

  ///
  void SetOwnSmoother (int os = 1);
  ///
  void SetOwnProlongation (int op = 1);
  ///
  void SetOwnCoarseGridPreconditioner (int oc = 1);
  ///
  void SetUpdateAll (int ua = 1);
  ///
  void SetUpdateAlways (bool ua = 1) { update_always = ua; }
  ///
  virtual void Update ();

  ///
  virtual void Mult (const BaseVector & x, BaseVector & y) const;

  ///
  void MGM (int level, BaseVector & u, 
	    const BaseVector & f, int incsm = 1) const;
  ///
  virtual BaseVector * CreateVector () const
  { return biform.GetMatrix().CreateVector(); }
  
  ///
  const Smoother & GetSmoother() const
  { return *smoother; }
  ///
  Smoother & GetSmoother()
  { return *smoother; }
  ///
  const Prolongation & GetProlongation() const
  { return *prolongation; }


  virtual int VHeight() const
  {
    return biform.GetMatrix().Height();
  }

  virtual int VWidth() const
  {
    return biform.GetMatrix().VWidth();
  }


  virtual void MemoryUsage (ARRAY<MemoryUsageStruct*> & mu) const;
};







///
class TwoLevelMatrix : public BaseMatrix
{
  ///
  const BaseMatrix * mat;
  ///
  const BaseMatrix * cpre;
  ///
  BaseJacobiPrecond * jacsmoother;
  ///
  Smoother * smoother;
  ///
  int level;
  ///
  int smoothingsteps;
  ///
  bool own_smoother;
public:
  ///
  TwoLevelMatrix (const BaseMatrix * amat, 
		  const BaseMatrix * acpre, 
		  Smoother * asmoother, int alevel);
  ///
  ~TwoLevelMatrix ();

  ///
  void FreeMem(void);
  ///
  virtual void Mult (const BaseVector & x, BaseVector & y) const;
  ///
  virtual BaseVector * CreateVector () const;
  ///
  virtual ostream & Print (ostream & s) const;
  ///
  void SetSmoothingSteps(int ass) { smoothingsteps = ass; }
  ///
  void SetOwnSmoother (bool aos) { own_smoother = aos; }
  ///
  const Smoother & GetSmoother() const
  { return *smoother; }
  ///
  Smoother & GetSmoother()
  { return *smoother; }
  ///
  virtual void Update();

  virtual int VHeight() const
  {
    return mat->Height();
  }

  virtual int VWidth() const
  {
    return mat->VWidth();
  }
  
  virtual void MemoryUsage (ARRAY<MemoryUsageStruct*> & mu) const;
};


#endif
