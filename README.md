# Final_Project

\documentclass[twoside]{article}
\usepackage{fullpage}
\usepackage[pdftex]{graphicx}
\usepackage{wrapfig}
\usepackage{amsmath}
\usepackage{hyperref}
\usepackage{sectsty}
%\sectionfont{\fontsize{13}{15}\selectfont}
\usepackage{fixltx2e}
\usepackage{fancyhdr}
\pagestyle{fancy}
\fancyhead{}
\fancyfoot{}
\renewcommand{\headrulewidth}{0pt}
\fancyfoot[LO]{\emph{Hall - CSI 370}}
\fancyfoot[LE]{\emph{Research 1 - Project}}
\fancyfoot[R] {\thepage}
\newenvironment{code}{\fontfamily{lmtt}\selectfont}{}
\date{}

\begin{document}

\title{CSI 370 Computer Architecture \\ Research 1 - Project}

\author{Liam J. Cannon | Ryan R. Lavigne \\
Champlain College\\
liam.cannon@ - ryan.lavigne@
}

\maketitle
\renewcommand{\labelitemi}{$\diamond$}

\begin{itemize}
\textbf{What?}\\
\\
    \item A program that utilizes both sockets and assembly was something that was a very attractive idea to both Ryan and I. Branching off from some of our previous projects from this semester and last seemed like a decent idea. Our idea for the project is to utilize c++ sockets and inline assembly to create a chat program that encrypts using some of the concepts learned in class to send and receive messages safely. The end goal is to be able to send both chats and files over the CPP network using an encryption of our own.\\
    \\
\textbf{Why?}\\
\\
    \item Sockets were something that intrigued the both of us when they were introduced last year in network programming. So after some considerations for different ideas we ended up choosing to try and integrate them with some inline assembly. The idea for this was something we hadn't heard come up as any examples during class of previous projects making it a small amount more enticing. Another small reason for our combined interest is a small shared interest in cryptography which led us to the conclusion for this project choice.\\
    \\
\textbf{How?}\\
\\
    \item Starting off, we'll need to put together a basic network capable of sending and receiving both text and files. This can be done fairly quickly and without issue. After that is accomplished, we're going to go about developing a system for the encryption in an attempt to make it semi difficult to decrypt without knowing any background information. We intend on using some combination of XOR, Bit-shifting, Rotations, and Keys. At this time our full strategy isn't known, but as we move further through the project it'll come to fruition.   \\
    \\
\textbf{Challenges?}\\
\\
    \item Some of the challenges that we are going to face are the following: 
    Liam uses GAS and Ryan uses MASM one of us will have to change to the others to be able to work together on the project. That will be some of the challenges. Some of the other challenges that will probably come up, Since neither one of us has used sockets in C++ that will be a learning curve. Along with making sure the client can send and receive messages. One of our biggest challenges is going to be developing our own encryption. There are so many different types of encryption out there today that making our one unique encryption seems slightly difficult. It is our full intention however, to put something together that is complex and functional. 

\end{itemize}
\end{document}
