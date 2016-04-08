\ProvidesClass{application}[2014/03/04 v1.0 Simple application class]
\LoadClass{article}
\usepackage{xeCJK}

\setCJKmainfont{微软雅黑}

\RequirePackage[T1]{fontenc}
\RequirePackage{lmodern}
\renewcommand*\familydefault{\sfdefault}
\RequirePackage{sfmath}
\RequirePackage{microtype}
\RequirePackage[utf8]{inputenc}

%\usepackage{cfr-lm}

\RequirePackage{listings}
\lstset{language=java}

\RequirePackage[ngerman]{babel}
%\RequirePackage[english]{babel}
%\selectlanguage{English}
%\selectlanguage{german}
\RequirePackage[margin=2cm,a4paper]{geometry}

\RequirePackage{graphicx}
\graphicspath{{./}{../}{images/}}

%\RequirePackage[pdftex,
	%unicode=true,
	%breaklinks=true,
	%colorlinks=true,
	%linkcolor=black,
	%citecolor=black,
	%urlcolor=black]{hyperref}

\renewcommand{\baselinestretch}{1.1}
\setlength{\parindent}{0pt}
%\setlength{\parskip}{5pt}

\newcommand{\titleabstand}{\hspace{3cm}}
\newcommand{\RNum}[1]{\uppercase\expandafter{\romannumeral #1\relax}}

\renewcommand{\maketitle}{
\begin{titlepage}
	\thispagestyle{empty}
  %\begin{minipage}[t]{1.0\textwidth}
%	\includegraphics[scale=0.4]{images/logo.jpg}
%  \end{minipage}
  \begin{minipage}[t]{1.0\textwidth}
  \vspace{-0.7cm}
	\hfill
	\includegraphics[scale=0.23]{images/FDU_logo.png}
  \end{minipage}

 \begin{minipage}{\textwidth}
  \ \\
  \vspace{0.5cm}

  \begin{minipage}[t]{1.0\textwidth}
  	\begin{flushright}
  	\Large{软件设计和开发}\\
	\Huge{Mini Project\\设计与测试报告}\\
	\Large 2016 Spring
  	\end{flushright}
    \end{minipage}
  %\hspace{1.8cm}
\end{minipage}

\hspace{3cm}
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak
\linebreak

\Large
\begin{flushright}
李琛\\
微电子科学与工程\\
\end{flushright}

\vspace{-0.9cm}
\begin{flushleft}
\href{mailto:13307130163@fudan.edu.cn}{13307130163@fudan.edu.cn}
\end{flushleft}

\end{titlepage}
}
