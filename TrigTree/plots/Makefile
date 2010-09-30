include Makefile.arch

#------------------------------------------------------------------------------

ANEXECO       = trigEff.$(ObjSuf)
ANEXECS       = trigEff.$(SrcSuf)
ANEXEC        = trigEff(ExeSuf)

ANEXE2CO      = ZeePlots.$(ObjSuf)
ANEXE2CS      = ZeePlots.$(SrcSuf)
ANEXE2C       = ZeePlots(ExeSuf)

ANEXE3CO      = ZeePlotsSlim.$(ObjSuf)
ANEXE3CS      = ZeePlotsSlim.$(SrcSuf)
ANEXE3C       = ZeePlotsSlim(ExeSuf)

HELPERSO      = helpers.$(ObjSuf)
HELPERSS      = helpers.$(SrcSuf)
HELPERSSO     = helpers.$(DllSuf)

OBJS          = $(ANEXECO) $(HELPERSO) $(ANEXE2CO) $(ANEXE3CO)

LIBS          += helpers.so

PROGRAMS      = $(HELPERSSO) $(ANEXEC) $(ANEXE2C) $(ANEXE3C)

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)
.PHONY:   

all:            $(PROGRAMS)

helpers:       $(HELPERSSO)
$(HELPERSSO):  $(HELPERSO)
		$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(EXPLLINKLIBS) $(OutPutOpt)$@
		$(MT_DLL)

$(ANEXEC):      $(ANEXECO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(ANEXE2C):     $(ANEXE2CO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(ANEXE3C):     $(ANEXE3CO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

clean:
		@rm -f $(OBJS) $(TRACKMATHSRC) core

distclean:      clean
		@rm -f $(PROGRAMS) $(EVENTSO) $(EVENTLIB) *Dict.* *.def *.exp \
		   *.ps *.so *.lib *.dll *.d *.log *~ .def so_locations
		   
install:
		@cp runAnalysis ~/bin
		@cp *.so ~/lib
		@echo runAnalysis installed.


.SUFFIXES: .$(SrcSuf)

.$(SrcSuf).$(ObjSuf):
	$(CXX)  $(CXXFLAGS) -c $<

