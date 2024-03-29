
%*********************************************************************
%
% thesis.cls
%
% Astronomy thesis DOCUMENT STYLE for LaTeX2e
%    -- See bottom for AASTeX -> thesis style conversion instructions
%
% Created: 1991: Siebers
%
% Modification History - 
%
%    July 1996, Eric Jensen -
%       Totally overhauled the style.  It was based on the Latex
%       ``book'' style (and still is).  Before the whole file was
%       basically a copy of the Latex 2.09 book.sty file, with a few
%       things changed here and there.  Now we load the Latex2e book
%       class at the beginning, and then change only those things which
%       need to be changed (which turns out to be not very much).  A
%       modified version of the AASTeX style is loaded which defines
%       all of the commands and environments used there.
%
%       The ``thesis'' style now can be invoked with various options,
%       including type sizes (10pt, 11pt, 12pt), and with various
%       margins: ``draft'' gives smaller margins and line spacing,
%       suitable for handing to your committee but without using a
%       whole ream of paper; ``final'' conforms to the grad school
%       margin requirements and is suitable for the final copy.
%       Default is \documentclass[12pt,final]{thesis}.
%
%    July 1996, Andrea Cox:
%       Added a \fixspacing command you can use if you want to guarantee
%         single-spaced figure and table captions in your final draft.
%         Insert this command within the caption environments (see below).
%       Redefined the table spacing within the \draftsize and \finalsize
%         commands, to ensure single-spacing of the table rows.  (See the
%         "arraystretch" option.)
%
%    October 1997, Jerry Acord:
%     *     Added the "sflabels" to the pool of options available in the
%       document declaration.  If "sflabels" is invoked (in the same
%       manner one invokes "12pt", "draft", etc.), then the labelling
%       for the Chapters, Sections, Table of Contents, and the page
%       numbers is done in a sans serif font.  If "sflabels" is not 
%       invoked, then these labels are by default in the roman font, 
%       the same as the rest of the text (boring!!).
%     *     Changed the definition of "\addconentsline" so as to incorporate
%       \labelfamily ("sflabels") when generating the TOC.
%     *     Added the environment "chabstract" for abstracts associated
%       with individual chapters (e.g. if the chapters are actually
%       journal articles).  The "chabstract" is treated like a section
%       in the TOC, and in the "chabstract" itself the word "Abstract"
%       is somewhat smaller than in the true "abstract" environment.  
%       The "abstract" environment generates a chapter-like entry in 
%       the TOC.  Of course these things can be easily modified in this 
%       file.
%     *     Added the commands \ttlpage and \cpypage which when invoked
%       with a few commands will produce the title page and copyright
%       page automatically, without having to type in all the info
%       onesself.  See below for more info.
%     *     Added the "umiabstract" environment, which will produce the 
%       UMI abstract required by the graduate school.  Actually, you
%       have to produce it, this environment just formats it properly.
%       It requires \author, \title, \adviser, and \adviserrank, as well 
%       as the file name of your UMI abstract (less than 350 words!).
%       Note that \adviser should be of the form "Bob Dole", and 
%       \adviserrank is of the form "Professor".
%       Remember, the UMI abstract is _not_ part of your thesis!!!
%       USAGE: \begin{umiabstract} Abstract text \end{umiabstract}
%       (Ideally this is in a file separate from your master.tex file
%       for your thesis; it still REQUIRES the thesis document class!)
%
%   (Added "compact" option, so if you choose "compact" in place of
%   "final" or "draft," you'll end up with something that is single-
%   spaced and more suited for printing lots of copies e.g. to
%   distribute to your neighbors as sleeping aides.  It produces
%   something more like a preprint/reprint, rather than the double-
%   spaced crap that Memorial Library wants. 15 Dec. 1997 Jerry Acord)
%
%    December 1999, Peter Erwin:
%     *     Fixed copyright page format (invoked by \cpypage) to
%       better match Graduate School guideline; important change was
%       moving the year to the end of the copyright line.
%     *     Fixed UMI abstract (now called ``Bell & Howell'' abstract,
%       apparently, but the current forms still say UMI) -- killed
%       generation of page numbers by changing \thispagestyle{thesis}
%       to \thispagestyle{empty}.  Added an additional \this...{empty}
%       before the ``Advisor'' text in case abstract runs to two
%       pages, which it usually does; initial \this...{empty} only
%       works for the first page!
%     *     Note that Graduate School format requirements have
%       changed somewhat: 10 point type is now acceptable!
%       (12 pt type is still acceptable, of course, if you have
%       a strong desire to use up lots of paper.)
%     *     Desiderata: A really ambitious person might want to
%       adapt this style to make use of AASTeX version 5.0 macros,
%       so that future ApJ/AJ/etc. papers can be easily incorporated
%       into a thesis.  However, one probably wants to wait until
%       the bugs in v5.0 are fixed....
%
%    August 2000, Chris Dolan:
%       Added AASTeX V5.0-like citation commands
%       Added notes to the bottom of this file on how to convert a
%          AASTeX paper into a thesis chapter
%       Minor fix in \tablecomments
%       Tweaked compact style (Note: compact style still doesn't space
%          footnotes and tablenotes right.  I don't know how to fix
%          this...)
%
%    February 2001, Ty DeYoung
%       Imported to the Physics Department
%       Commented out AAS-style citations, symbols, which conflict 
%           with AMS-style definitions
%       Assorted tweaks for spacing, etc.
%
%    January 2005, Liang Li
%       oneside or twoside? that's the question. You can use both, search for 'oneside'.
%       
%*********************************************************************

%\ProvidesClass{thesis}[1996/07/09 v1.0 UW Astronomy thesis document class]
%\ProvidesClass{thesis}[1997/10/05 v1.1 UW Astronomy thesis document class]
%\ProvidesClass{thesis}[2000/06/27 v1.2 UW Astronomy thesis document class]
\ProvidesClass{thesis}[2001/02/14 v1.3 UW Physics thesis document class]

\newcommand\@myptsize{12}       % Default point size is 12.
        % Note that the Graduate School now permits 10 point type
        % as well.

\newcommand\singlespace{\renewcommand\baselinestretch{1.2}}
\newcommand\doublespace{\renewcommand\baselinestretch{1.8}}
\newcommand\fixspacing{\setlength{\baselineskip}{1.2em}}
        % Insert this command in your table and figure captions if you
        % want them to be single-spaced in the final version.
\newcommand\fixtabspacing{\renewcommand\arraystretch{0.2}}

\newcommand\draftsize{
  \setlength\overfullrule{5pt}
  \setlength{\textwidth}{6.5in}
  \setlength{\oddsidemargin}{0in}
  \setlength{\textheight}{8.7in}
  \setlength{\topmargin}{0.in}
  \setlength{\parindent}{0.4in}
  \setlength{\parskip}{0.1in}
  \renewcommand\baselinestretch{1.2}
  \renewcommand\arraystretch{0.9}       % Sets the row spacing of the tables
                                        %   to 0.9 times the "baselinestretch"
  }

\newcommand\finalsize{
  \setlength\overfullrule{0pt}
  \setlength{\textwidth}{6.05in}
  \setlength{\oddsidemargin}{0.35in}
%  \setlength{\oddsidemargin}{0.5in}
  \setlength{\evensidemargin}{0in}
  \setlength{\textheight}{8.45in}
  \setlength{\topmargin}{0in}
  \setlength{\headsep}{0.225in}
  \setlength{\parindent}{0.4in}
  \renewcommand\baselinestretch{1.8}
  \renewcommand\arraystretch{0.6}       % Sets the row spacing of the tables
                                        %   to 0.6 times the "baselinestretch"
  }


\newcommand\compactopts{
  \setlength\overfullrule{0pt}
  \setlength{\textwidth}{6in}
  \setlength{\oddsidemargin}{0.5in}
  \setlength{\evensidemargin}{0in}
  \setlength{\textheight}{8.5in}
  \setlength{\topmargin}{0in}
  \renewcommand\baselinestretch{1.0}    % Standard value=1.0.
  \renewcommand\arraystretch{1.0}       % Sets the row spacing of the tables
                                        %   as a multiple of "baselinestretch"
  }

% If the option "sflabels" is used in the document declaration, then 
% the sans serif font family will be used for chapters, sections, the
% table of contents, and page numbers.  In practice, this is implemented 
% by defining the command "\labelfamily" to be either "\rmfamily" 
% (default) or "\sfamily" (if "sflabels" is invoked).
%
\newcommand{\labelfamily}{\rmfamily}
\DeclareOption{sflabels}{\renewcommand{\labelfamily}{\sffamily}}


\DeclareOption{10pt}{\renewcommand\@myptsize{10}}
\DeclareOption{11pt}{\renewcommand\@myptsize{11}}
\DeclareOption{12pt}{\renewcommand\@myptsize{12}}

% The next two options, ``draft'' and ``final'' determine how big the
% margins are.  ``Final'' is the default.  Their processing is deferred
% to the end of the class so that the ``book'' class below doesn't step
% on the definitions.

% Use the draft option for printing to save paper when you don't have
% to generate a copy that meets grad school requirements.
\DeclareOption{draft}{\AtEndOfClass{\draftsize}
                        \newif\ifkompakt
                        \kompaktfalse}

% The ``final'' option will pass the Margin Lady.
\DeclareOption{final}{\AtEndOfClass{\finalsize}
                        \newif\ifkompakt
                        \kompaktfalse}

% The ``compact'' option actually looks halfway decent
\DeclareOption{compact}{\AtEndOfClass{\compactopts}
                        \newif\ifkompakt
                        \kompakttrue}

% If they don't specify "draft" or "final", use final for the default:
\ExecuteOptions{final}

\ProcessOptions

% Construct a point-size option name (e.g. ``12pt'') based on the
% options specified above:

\def\ptoption{\@myptsize pt}

% Load the Latex2e ``book'' class to get the basic definitions:
%\ifkompakt\LoadClass[twoside,\ptoption]{book}\else\LoadClass[oneside,\ptoption]{book}\fi
% switch these between two lines to get oneside or twoside document, default is to use 'twoside'
\LoadClass[oneside,\ptoption]{book}
%\LoadClass[twoside,\ptoption]{book}

% This style file defines AASTeX V5.0-like citation commands
%\RequirePackage{natbib}% 

% Also input a modified version of the AAS MS style to add various
% symbol definitions (note this was written for AASTeX 4.0 and has
% been partially updated for AASTeX 5.0.  Some minor differences may
% remain...)
%\input{thesis_aasms.sty}

%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                         %
%  \ttlpage and \cpypage  %
%                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% I envision the commands \ttlpage and \cpypage invoked thusly:
% (in your master.tex file:)
%
%    \title{My Wonderful Graduate Student Life}
%    \author{Bob Dole}
%    \year{1697}
%    \ttlpage
%    \cpypage
%
% (that's it.)
%
% The command \ttlpage will produce the standard thesis title page.
% It requires 3 variables be set: 1) \mytitle, 2) \myname, and 3) 
% \myyear.  These should be set in your master.tex file prior to 
% invoking \ttlpage.  Nota Bene: you may have to provide your own 
% line breaks if your thesis title is too long.  
% The command \cpypage will produce the standard thesis copyright page.
% It also requires \myname and \myyear be set.
%                                         -- Jerry Acord, October 1997
%
% Redefine the \title, \author, \year commands to simply repeat 
% their arguments, and define \adviser similarly:
%% \renewcommand{\title}[1]{\renewcommand{\title}{#1}}
%% \renewcommand{\author}[1]{\renewcommand{\author}{#1}}
%% \renewcommand{\year}[1]{\renewcommand{\year}{#1}}
\newcommand{\mytitle}[1]{\renewcommand{\title}{#1}}
\newcommand{\myauthor}[1]{\renewcommand{\author}{#1}}
\newcommand{\myyear}[1]{\renewcommand{\year}{#1}}
\newcommand{\adviser}[1]{\renewcommand{\adviser}{#1}}
\newcommand{\adviserrank}[1]{\renewcommand{\adviserrank}{#1}}

\newcommand{\ttlpage}{%
     %\thispagestyle{empty} \vspace*{1.0cm} \begin{center}
     %\thispagestyle{empty}  {\ } \vfill \begin{center}
     %\setfoot{\small \itshape \LARGE \centerline{ DRAFT \today }}{\ } \vfill \begin{center}
     %\setfoot{\small \itshape \LARGE \centerline{ }}{\ } 
     \vfill \begin{center}
     \textsc{\large \title}\\ \vspace{1.5cm} \textit{by}\\ \vspace{1.5cm}
     \textsc{\author}\\ \vspace{1.5cm} A dissertation submitted in partial
     fulfillment of the\\ requirements for the degree of\\
     \vspace{1.5cm} \textsc{Doctor of Philosophy}\\ \textsc{(Physics)}\\ 
     \vspace{1.5cm} \textit{at the}\\ \vspace{0.5cm} 
     \textsc{University of Wisconsin -- Madison}\\ \vspace{0.5cm}
     {\year}\\ \end{center} \thispagestyle{empty} \vfill\eject
     %{\year}\\ \end{center} \vfill\eject
     % Make page inside front cover be really blank (get rid of page number)
     \ifkompakt\thispagestyle{empty}\newpage\else\fi
     }%

% Copyright page amended to fit with Graduate School format:
% ``(c) Copyright by Bob Dole 1697\\All Rights Reserved''
%                                         -- Peter Erwin, December 1999
\newcommand{\cpypage}{%
     \thispagestyle{empty} {\ } \vfill \begin{center} \copyright\ 
     Copyright by \author\ \year \\ All Rights Reserved \end{center} \eject
     }%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                %
% Define the UMI abstract-generating environment %
%                                                %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% December 1999, Peter Erwin: I added two \thispagestyle{empty}
%   commands to ensure that the UMI abstract has no page numbers;
%   the second command seems to be necessary to ensure that the
%   second page (anything but a really short abstract will run
%   two pages, at least in 12 pt type) has no page numbers, either.
%
% May 2001, Ty DeYoung: Modified slightly to conform to Grad School format
\newenvironment{umiabstract}{%
  \thispagestyle{empty} \begin{center} \textsc{\Large \title} \\ \vspace{0.6cm}
  {\large \author}\\ \vspace{0.15cm} 
  {\large Under the supervision of \adviserrank\ \adviser \\
  At the University of Wisconsin --- Madison} \end{center}
  \vspace{2em}}%
%{\thispagestyle{empty} \vfill {\hfill \adviser\ (Adviser)}}



% Fix the "chapter" command so it doesn't change the page style for
% the first page of each chapter (took out "\thispagestyle{plain}")

\renewcommand\chapter{
        \if@openright\cleardoublepage\else\clearpage\fi
        \global\@topnum\z@
        \@afterindentfalse
        \secdef\@chapter\@schapter
        }

% Slight redefinition of two commands which change the size of the chapter
% headings.  If you want different sizes, change the two commands below.

% This controls the size of chapter titles.
\newcommand\chaptertitlesize{\LARGE}

% This controls the size of the heading for table of contents, list
% of figures, and list of tables.
\newcommand\toctitlesize{\LARGE}

% Here's where those sizes get implemented:
\def\@makechapterhead#1{%
  \vspace*{25\p@}%
  {\parindent \z@ \raggedright \normalfont
    \ifnum \c@secnumdepth >\m@ne
      \if@mainmatter
        \chaptertitlesize\bfseries\labelfamily \@chapapp{} \thechapter
        \par\nobreak
        \vskip 20\p@
      \fi
    \fi
    \interlinepenalty\@M
    \chaptertitlesize\bfseries\labelfamily #1\par\nobreak
    \vskip 40\p@
  }}

\def\@makeschapterhead#1{%
  \vspace*{50\p@}%
  {\parindent \z@ \raggedright
    \normalfont
    \interlinepenalty\@M
    \toctitlesize\bfseries\labelfamily  #1\par\nobreak
    \vskip 40\p@
  }}

% Changed the definitions of section, subsection, and subsubsection 
% here to allow more control of the spacing around them.
% --- And to incorporate \labelfamily ...

\newlength{\beforesecskip}
\newlength{\aftersecskip}
\beforesecskip = 2.ex           % Originally -1.ex
\aftersecskip = 1.ex            % Originally -2.ex
\def\section#1{
\vspace*{\beforesecskip}
\@startsection {section}{1}{\z@}
 {0ex}{0.001ex}{\labelfamily\large\bfseries}{#1}
\vspace*{\aftersecskip}}
\def\subsection#1{\vspace*{\beforesecskip}
\@startsection{subsection}{2}{\z@}
 {0ex}{0.001ex}{\labelfamily\normalsize\bfseries}{#1}\vspace*{\aftersecskip}}
\def\subsubsection#1{\vspace*{\beforesecskip}
\@startsection{subsubsection}{3}{\z@}
 {0ex}{0.001ex}{\labelfamily\normalsize\bfseries}{#1}\vspace*{\aftersecskip}}


% Change \addcontentsline to incorporate \labelfamily -- this is
% modified from the file "latex.ltx"  -- October 1997, Jerry Acord

\def\addcontentsline#1#2#3{%
  \addtocontents{#1}{\protect\contentsline{#2}%
     {\labelfamily #3}{\labelfamily \thepage}}}

% Also give the user the opportunity to set the page footer and header
% from within the document.  By default, the header does the page
% numbering, but you might want to put extra info in the footers.  One
% example: \setfoot{\tiny \hss DRAFT \today\hss} will create a footer
% with the word "DRAFT" and today's date in small print centered at the
% bottom of each page - handy when dealing with multiple drafts and
% trying to remember what you printed when.

\def\setfoot#1{\def\@evenfoot{#1}\let\@oddfoot=\@evenfoot}
\def\sethead#1{\let\savehead=\@evenhead\def\@evenhead{#1}
           \let\@oddhead=\@evenhead\let\oldhead=\savehead}

% Appendix for an individual chapter; reset the section and equation
% counters, saving the old values to be restored with the
% \endchapappendix command.

\def\chapappendix{\par
\setcounter{section}{0}
\setcounter{subsection}{0}
\setcounter{equation}{0}
\let\savesection=\thesection
\let\saveequation=\theequation
\def\thesection{\Alph{section}}
\def\theequation{\hbox{\normalsize\Alph{section}\arabic{equation}}}
}

% End of chapappendix def.
\def\endchapappendix{\let\thesection=\savesection
  \let\theequation=\saveequation
}

% If you want a footnote with no marker, just the footnote text at the
% bottom of the page, use \blanknote{text}

\long\def\@makeblankfntext#1{\parindent 1em\noindent
            \hbox to 1.8em{\hss$^{}$}#1}

\def\blanknote#1{\let\savefn=\@makefntext
\let\@makefntext=\@makeblankfntext
\footnotetext{#1}\let\@makefntext=\savefn}

% Allow hyphenation:
   \pretolerance=100
   \tolerance=200

% Change tocdepth and secnumdepth; these control which level of
% sectioning will be numbered in the text and will be included in the
% table of contents; default is 2, which is subsection for this style;
% change to 3 to include subsubsections:

\setcounter{tocdepth}{3}
\setcounter{secnumdepth}{3}



%    ****************************************
%    *             FOOTNOTES                *
%    ****************************************
%
% \footnoterule is a macro to draw the rule separating the footnotes from
% the text.  It should take zero vertical space, so it needs a negative
% skip to compensate for any positive space taken by the rule.  (See
% PLAIN.TEX.)

% Changed 7/1/96 allow a little more space around footnotes; the
% default vertical spacing is too crowded for my tastes; ELNJ
\renewcommand\footnoterule{%
  \hrule\@width.4\columnwidth
  \kern8.6\p@}

% Added some space between notes -- Grad School require double space 
%  between notes, single space within (use \fixspacing in the note for that)
\setlength{\footnotesep}{2ex}


%%%%%%%%%%%%%%%%%%%%%%%%%
%                       %
%    ABSTRACT STYLES    %
%                       %
%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Not sure if these abstract definitions are actually useful, but
% it doesn't hurt to leave them here...
%
% October 1997, Jerry Acord: I've found it necessary to change 
%   \thispagestyle to "thesis" from "plain" to ensure the page
%   numbers go in the upper right as required (and that they 
%   conform to \labelfamily just like all the other page numbers).
%
%   I commented out the entry for the (blank) section number of
%   the abstract; removing it justifies the "Abstract" title with
%   the chapter numbers on the left-hand margin.
%

\def\abstractpage{\@restonecolfalse\if@twocolumn\@restonecoltrue\onecolumn
     \else \newpage \fi \thispagestyle{thesis}}
\def\endabstractpage{\if@restonecol\twocolumn \else \newpage \fi}

\def\abstract{\abstractpage
     \addcontentsline{toc}{chapter}{%
%     \protect\numberline{\ }  % uncomment to indent Abstract in TOC
     Abstract}
%     \begin{center}
     {\labelfamily \bfseries \Large \noindent Abstract}
     \vspace{0.5cm}
%\end{center}
}

\def\endabstract{\par\vfil\null\endabstractpage}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  Acknowledgements
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\acknow{\abstractpage
\newlength{\dummyA}\setlength{\dummyA}{\parindent}
\newlength{\dummyB}\setlength{\dummyB}{\parskip}
     \setlength{\parindent}{0in}
     \setlength{\parskip}{0.15in}
     \addcontentsline{toc}{chapter}{%
%     \protect\numberline{\ }  % uncomment to indent Acknow in TOC
     Acknowledgements}
     \vspace{1cm}
%     \begin{center}
     {\labelfamily \bfseries \Large Acknowledgements}
%\end{center}
}
\def\endacknow{\par\vfil\null\endabstractpage
\setlength{\parindent}{\dummyA}
\setlength{\parskip}{\dummyB}
}
%
% The \chabstract environment is used for an abstract for an 
% individual chapter, rather than the global \abstract at the
% beginning of the manuscript.  This is particularly useful 
% if your chapters are actually journal articles with individual
% abstracts of their own.  Note that the \chabstract entry in 
% the table of contents appears as an unnumbered section, 
% whereas the \abstract appears as an unnumbered chapter.
%                                    - October 1997, Jerry Acord
%

\def\chabstractpage{\@restonecolfalse\if@twocolumn\@restonecoltrue\onecolumn
     \else \newpage \fi \thispagestyle{thesis}}
\def\endchabstractpage{\if@restonecol\twocolumn \else \newpage \fi}

\def\chabstract{\chabstractpage
     \addcontentsline{toc}{section}{Abstract}
     \null\vfil
     \begin{center}
     {\labelfamily \bfseries \large Abstract}
     \end{center}}
\def\endchabstract{\par\vfil\null\endchabstractpage}


%    ****************************************
%    *            PAGE STYLES               *
%    ****************************************
%
% The page style 'foo' is defined by defining the command \ps@foo.  This
% command should make only local definitions.  There should be no stray
% spaces in the definition, since they could lead to mysterious extra
% spaces in the output.
%
% The \ps@... command defines the macros \@oddhead, \@oddfoot,
% \@evenhead, and \@evenfoot to define the running heads and
% feet---e.g., \@oddhead is the macro to produce the contents of the
% heading box for odd-numbered pages.  It is called inside an \hbox of
% width \textwidth.
%
% To make headings determined by the sectioning commands, the page style
% defines the commands \chaptermark, \sectionmark, ... , where
% \chaptermark{TEXT} is called by \chapter to set a mark, and so on.
% The \...mark commands and the \...head macros are defined with the
% help of the following macros.  (All the \...mark commands should be
% initialized to no-ops.)
%
% MARKING CONVENTIONS:
% LaTeX extends TeX's \mark facility by producing two kinds of marks
% a 'left' and a 'right' mark, using the following commands:
%     \markboth{LEFT}{RIGHT} : Adds both marks.
%     \markright{RIGHT}      : Adds a 'right' mark.
%     \leftmark  : Used in the \@oddhead, \@oddfoot, \@evenhead or \@evenfoot
%                  macro, gets the current 'left'  mark.  Works like TeX's
%                  \botmark command.
%     \rightmark : Used in the \@oddhead, \@oddfoot, \@evenhead or \@evenfoot
%                  macro, gets the current 'right'  mark.  Works like TeX's
%                  \firstmark command.
% The marking commands work reasonably well for right marks 'numbered
% within' left marks--e.g., the left mark is changed by a \chapter command and
% the right mark is changed by a \section command.  However, it does
% produce somewhat anomalous results if two \bothmark's occur on the same page.
%
% Commands like \tableofcontents that should set the marks in some page styles
% use a \@mkboth command, which is \let by the pagestyle command (\ps@...)
% to \markboth for setting the heading or \@gobbletwo to do nothing.

\mark{{}{}}   % Initializes TeX's marks

%    ****************************************
%    *     THESIS PAGE STYLE                *
%    ****************************************
%
%
% Dec 1997 -- Acord -- added the if/else clause to change the
%   page style definition if the "compact" option is being used.
%   The "compact" page style has chapters & sections in the 
%   page headers in addition to the page numbers.
%
\ifkompakt%
\def\ps@thesis{%
      \let\@oddfoot\@empty\let\@evenfoot\@empty
      %% The following used to put the chapter title by the bold-faced
      %% page number, but it looked terrible, IMHO   --- Dolan
      %\def\@evenhead{\labelfamily\bfseries\thepage\hfil\leftmark}%
      %\def\@oddhead{\labelfamily\bfseries\rightmark\hfil\thepage}%
      \def\@evenhead{\labelfamily\thepage\hfil}%
      \def\@oddhead{\labelfamily\hfil\thepage}%
      \let\@mkboth\markboth
    \def\chaptermark##1{%
      \markboth {{%
        \ifnum \c@secnumdepth >\m@ne
          \if@mainmatter
            \@chapapp\ \thechapter. \ %
          \fi
        \fi
        ##1}}{}}%
    \def\sectionmark##1{%
      \markright {{%
        \ifnum \c@secnumdepth >\z@
          \thesection. \ %
        \fi
        ##1}}}}%
\else%
\def\ps@thesis{\let\@mkboth\@gobbletwo%
    \def\@oddfoot{}\def\@evenfoot{}%
    \def\@oddhead{\labelfamily\hfil\thepage}%
    \def\@evenhead{\labelfamily\thepage\hfil}}%
\fi

%    ****************************************
%    *   MORE CRAP FOR THE COMPACT STYLE    *
%    ****************************************
%
% Changed the \chapter* commands to \section* commands,
% so that the list of figures and list of tables are
% treated as sections in the TOC, rather than separate
% chapters (duh!).  Otherwise this is more or less
% straight from book.cls (except I removed the \MakeUppercase
% command and added \labelfamily, of course).
% -- Acord 12/97

\ifkompakt%
\renewcommand\tableofcontents{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\contentsname
        \@mkboth{%
           \labelfamily\contentsname}{\labelfamily\contentsname}}%
    \@starttoc{toc}%
    \if@restonecol\twocolumn\fi
    }
%\renewcommand\listoffigures{%
%    \if@twocolumn
%      \@restonecoltrue\onecolumn
%    \else
%      \@restonecolfalse
%    \fi
%    \@starttoc{lof}%
%    \if@restonecol\twocolumn\fi
%    }
%\renewcommand\listoftables{%
%    \if@twocolumn
%      \@restonecoltrue\onecolumn
%    \else
%      \@restonecolfalse
%    \fi
%    \@starttoc{lot}%
%    \if@restonecol\twocolumn\fi
%    }
\else\fi

%    ****************************************
%    *           INITIALIZATION             *
%    ****************************************
%
% Default initializations

\ps@thesis                  % 'thesis' page style
\pagenumbering{arabic}      % Arabic page numbers
\onecolumn                  % Single-column.
\if@twoside\else\raggedbottom\fi % Ragged bottom unless twoside option.

%% END %%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%        How to convert a AASTeX 5.0 paper into a thesis chapter        %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  (Note changing a AASTeX 4.0 paper to thesis style requires nearly
%  identical steps)
%
%  * Change paper title -> chapter title:
%        \title{text} -> \chapter[text]{text}
%           (the two versions in \chapter are for what shows up in the
%           table of contents vs. what shows up at the start of the
%           chapter)
%  * Optional: Label the chapter   e.g. \label{ch2}
%  * Optional: Define a \FigDir (only if you are using the \figcaption
%        macro that CJD created
%  * Remove header stuff:
%        \documentclass (or \documentstyle for old papers)
%        \begin{document}
%        \slugcomment
%        \author, \affil, \email, \shortauthors, \shorttitle, etc
%        \keywords
%  * Optional: Move the \newcommand{}s to define.tex
%  * Change \begin{abstract} -> \begin{chabstract}  (ditto for \end{})
%  * Optional: change all citations and references to have, for
%        example, ``ch1'' in them.  That is, \citet{dol00} ->
%        \citet{ch1:dol00}.  This is to prevent instances of
%        ``Citation multiply defined'' or (even worse) a section
%        number multiply defined (i.e., what happens if you have
%        \label{sec:discussion} in both Chapters 1 & 2 and then you
%        have a \ref{sec:discussion} in Chapter 3?  Better to have
%        \ref{ch2:discussion}, I say)
%  * Move figures to their appropriate places in the text (usually
%        where you put the \placefigure{}s eariler)
%  * Put \fixspacing in every figure caption
%  * Put \fixspacing in every footnote
%  * Change \appendix to \chapappendix and add \endchapappendix
%  * Fix tables:
%        - Change all \\ to \nl  (Note someone should fix the thesis
%          style to allow either...) 
%        - Put \fixspacing in every \tablenotetext
%        - Remove the \rotate commands and put \begin{landscape} and
%          \end{landscape around the \begin,\end{deluxetable} commands
%          where needed
%  * Remove footer stuff:
%        \end{document}
%           (Note if you forget to do this you will get a very
%           mysterious error message like:
%           \end occurred when \iftrue on line 85 was incomplete)
