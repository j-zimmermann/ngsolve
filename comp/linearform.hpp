#ifndef FILE_LINEARFORM
#define FILE_LINEARFORM

/*********************************************************************/
/* File:   linearform.hh                                             */
/* Author: Joachim Schoeberl                                         */
/* Date:   25. Mar. 2000                                             */
/*********************************************************************/

namespace ngcomp
{

  /** 
      Linearform
  */

  class NGS_DLL_HEADER LinearForm : public NGS_Object
  {
  protected:
    ///
    shared_ptr<FESpace> fespace;
    ///
    Array<shared_ptr<LinearFormIntegrator>> parts;
    /// do the integration on independent meshes
    bool independent;
    /// print the assembled vector to testout
    bool print;
    /// print element vectos to testout
    bool printelvec;

    bool allocated;
    bool assembled;
    bool initialassembling;

    int cacheblocksize;
    /// output of norm of matrix entries
    bool checksum;

  public:
    ///
    LinearForm (shared_ptr<FESpace> afespace, 
		const string & aname, const Flags & flags);

    ///
    virtual ~LinearForm () { ; }
  
    ///
    shared_ptr<FESpace> GetFESpace() const { return fespace; }

    ///
    virtual LinearForm & AddIntegrator (shared_ptr<LinearFormIntegrator> lfi);

    LinearForm & operator+= (shared_ptr<LinearFormIntegrator> lfi)
    {
      return AddIntegrator(lfi);
    }

    ///
    shared_ptr<LinearFormIntegrator> GetIntegrator (int i) const
    {
      return parts[i]; 
    }

    const Array<shared_ptr<LinearFormIntegrator>> & Integrators() const
    {
      return parts;
    }

    ///
    int NumIntegrators () const
    {
      return parts.Size(); 
    }

    void SetIndependent (bool aindependent = true)
    { 
      independent = aindependent; 
    }

    ///
    virtual void Assemble (LocalHeap & lh) = 0;
    ///
    virtual void AllocateVector () = 0;

    virtual void CleanUpLevel() { ; }

    virtual bool IsAssembled () { return assembled; }
    bool InitialAssembling () { return initialassembling; }
    void SetNoInitialAssembling () { initialassembling = false; }


    ///
    virtual shared_ptr<BaseVector> GetVectorPtr() const = 0;
    virtual BaseVector & GetVector () const = 0;

    ///
    virtual string GetClassName () const
    {
      return "LinearForm";
    }

    void SetPrint (bool ap);
    void SetPrintElmat (bool ap);

    ///
    virtual void PrintReport (ostream & ost) const;
    ///
    virtual void MemoryUsage (Array<MemoryUsageStruct*> & mu) const;


    virtual void AddElementVector (FlatArray<int> dnums,
                                   FlatVector<double> elvec,
				   int cachecomp = -1);
    virtual void SetElementVector (FlatArray<int> dnums,
				   FlatVector<double> elvec);
    virtual void GetElementVector (FlatArray<int> dnums,
				   FlatVector<double> elvec) const;

    virtual void AddElementVector (FlatArray<int> dnums,
				   FlatVector<Complex> elvec,
				   int cachecomp = -1);
    virtual void SetElementVector (FlatArray<int> dnums,
                                   FlatVector<Complex> elvec);
    virtual void GetElementVector (FlatArray<int> dnums,
				   FlatVector<Complex> elvec) const;



    virtual void SetCacheBlockSize (const int size)
    {
      cacheblocksize = size;
    }
  };


  template <class SCAL>
  class NGS_DLL_HEADER S_LinearForm : public LinearForm
  {
  public:
    typedef SCAL TSCAL;

    ///
    using LinearForm::LinearForm;

    ///
    virtual void AddElementVector (FlatArray<int> dnums,
                                   FlatVector<SCAL> elvec,
                                   int cachecomp = -1) = 0;
    virtual void SetElementVector (FlatArray<int> dnums,
                                   FlatVector<SCAL> elvec) = 0;
    virtual void GetElementVector (FlatArray<int> dnums,
				   FlatVector<SCAL> elvec) const = 0;

    ///
    virtual void Assemble (LocalHeap & lh);
    void AssembleIndependent (LocalHeap & lh);
  };






  /// Template argument specifies vector type
  template <class TV>
  class NGS_DLL_HEADER T_LinearForm : public S_LinearForm<typename mat_traits<TV>::TSCAL>
  {
    ///
    shared_ptr<VVector<TV>> vec;
    typedef S_LinearForm<typename mat_traits<TV>::TSCAL> BASE;
  public:

    typedef typename mat_traits<TV>::TSCAL TSCAL;
    enum { HEIGHT = mat_traits<TV>::HEIGHT };

    ///
    //using S_LinearForm<TSCAL>::S_LinearForm;
    
    T_LinearForm(shared_ptr<FESpace> afespace, const string & aname,
        const Flags & flags)
        : BASE(afespace, aname, flags) { ; }
    
    ///
    virtual ~T_LinearForm ();

    ///
    virtual BaseVector & GetVector () const override { return *vec; }
    virtual shared_ptr<BaseVector> GetVectorPtr () const override { return vec; }

    ///
    virtual void AllocateVector () override;
    ///
    virtual void CleanUpLevel() override;

    ///
    virtual void AddElementVector (FlatArray<int> dnums,
				   FlatVector<TSCAL> elvec,
                                   int cachecomp = -1) override;
    virtual void SetElementVector (FlatArray<int> dnums,
                                   FlatVector<TSCAL> elvec) override;
    virtual void GetElementVector (FlatArray<int> dnums,
				   FlatVector<TSCAL> elvec) const override;
  };





  class ComponentLinearForm : public LinearForm
  {
    LinearForm * base_lf;
    int comp;
  public:
    ComponentLinearForm (LinearForm * abase_lf, int acomp, int ancomp);
    virtual LinearForm & AddIntegrator (shared_ptr<LinearFormIntegrator> lfi);

    virtual void AllocateVector () { cerr << "comp - allocate is illegal" << endl; }
    virtual void Assemble (LocalHeap & lh) { cerr << "comp - assemble is illegal" << endl; }
    virtual shared_ptr<BaseVector> GetVectorPtr() const
    {
      auto fes = dynamic_pointer_cast<CompoundFESpace> (base_lf->GetFESpace());
      return base_lf->GetVectorPtr()->Range(fes->GetRange(comp));
      // throw Exception ("comp - GetVectorPtr is illegal");
    }
    virtual BaseVector & GetVector () const 
    { throw Exception ("comp - GetVector is illegal"); }
  };



  extern NGS_DLL_HEADER shared_ptr<LinearForm> CreateLinearForm (shared_ptr<FESpace> space,
                                                                 const string & name,
                                                                 const Flags & flags);

}

#endif


