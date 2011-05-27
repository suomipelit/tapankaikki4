#include "EditorLevelIO.h"

#include "../common/CEditableLevel.h"
#include "../common/CGraphicsDevice.h"
#include "../common/error.h"
#include "../common/files.h"
#include "EditorDrawer.h"
#include "EditorGraphics.h"

#ifndef __unix__
#include <windows.h>
#include <commdlg.h>
#else
#include <gtk/gtk.h>
#include <string>

namespace {
	std::string tmp_filename;
	GtkWidget* file_widget;
	void file_callback(GtkWidget* w, GtkFileSelection* fs)
	{
		tmp_filename = gtk_file_selection_get_filename(fs);
		gtk_widget_destroy(file_widget);
	}

	void end_widget(GtkWidget* w, gpointer data)
	{
		gtk_main_quit();
	}
};
#endif

CEditorLevelIO::CEditorLevelIO(CEditableLevel* aLevel,SDL_SysWMinfo aWndInfo,CEditorDrawer* aDrawer, CEditorGraphics* aGraphics, CGraphicsDevice* aGD)
{
	iLevel = aLevel;
	iDrawer = aDrawer;
	iWndInfo = aWndInfo;
	iGraphics = aGraphics;
	iGD = aGD;

#ifdef __unix__
	gtk_init(0, 0);
#endif
}

CEditorLevelIO::~CEditorLevelIO(void)
{
}

bool CEditorLevelIO::LoadINI()
{
#ifndef __unix__

	OPENFILENAME ofn;       // common dialog box structure
	char szFile[FILENAME_MAX];       // buffer for file name
	char tmp[FILENAME_MAX];
	HWND hwnd;              // owner window
	int ret;

	hwnd = iWndInfo.window;

	_getcwd(tmp,FILENAME_MAX);
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(&szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.ini)\0*.INI\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE)
	{
		_chdir( tmp );
		try
		{
			std::string temp = getpath(ofn.lpstrFile);
			temp += "default.ini";
			iGraphics->Load(temp.c_str(),ofn.lpstrFile);
			iGD->SetPalette(*iGraphics->Palette(),256);
		}
		catch ( CFailureException& e )
		{
			error("Cannot open file (%s)!",e.what());
		}

		return true;
	}
	else
	{
		// TODO: Do nothing 
		ret = CommDlgExtendedError();
	}
	_chdir( tmp );
	return false; // if no INI loaded

#else	// __unix__
	tmp_filename.clear();
	file_widget = gtk_file_selection_new("Select INI-file");
	g_signal_connect(G_OBJECT(file_widget), "destroy", G_CALLBACK(gtk_main_quit), 0);
	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(file_widget)->ok_button), "clicked", G_CALLBACK(file_callback), (gpointer)file_widget);
	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(file_widget)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), G_OBJECT(file_widget));
	gtk_widget_show(file_widget);

	// ensure the dialog will not end up behind the editor window
	gdk_window_set_keep_above(file_widget->window, 1);
	gdk_window_raise(file_widget->window);
	gdk_window_show(file_widget->window);

	gtk_main();

	if (!tmp_filename.empty())
	{
		try
		{
			std::string temp = getpath(tmp_filename.c_str());
			temp += "default.ini";
			iGraphics->Load(temp.c_str(), tmp_filename.c_str());
			iGD->SetPalette(*iGraphics->Palette(),256);
		}
		catch( CFailureException& e )
		{
			error("Cannot open file (%s)!", e.what());
		}

		return true;
	}
	else
	{
		// smt?
	}
	return false;
#endif	// end __unix__
}

bool CEditorLevelIO::LoadLevel()
{
#ifndef __unix__

	OPENFILENAME ofn;       // common dialog box structure
	char szFile[FILENAME_MAX];       // buffer for file name
	char tmp[FILENAME_MAX];
	HWND hwnd;              // owner window
	int ret;

	hwnd = iWndInfo.window;

	_getcwd(tmp,FILENAME_MAX);
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(&szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.lev)\0*.LEV\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE)
	{
		_chdir( tmp );
		try
		{
			iLevel->Load(ofn.lpstrFile);
			std::string temp = getpath(ofn.lpstrFile);
			temp += "default.ini";
			std::string fname (ofn.lpstrFile);
			fname = fname.substr(0,fname.size()-4)+".ini";
			iGraphics->Load(temp.c_str(),fname.c_str());
			iGD->SetPalette(*iGraphics->Palette(),256);
		}
		catch ( CFailureException& e )
		{
			error("Cannot open file (%s)!",e.what());
		}

		return true;
	}
	else
	{
		// TODO: Do nothing 
		ret = CommDlgExtendedError();
	}
	_chdir( tmp );
	return false; // if no level loaded

#else	// __unix__
	tmp_filename.clear();
	file_widget = gtk_file_selection_new("Load level:");
	g_signal_connect(G_OBJECT(file_widget), "destroy", G_CALLBACK(gtk_main_quit), 0);
	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(file_widget)->ok_button), "clicked", G_CALLBACK(file_callback), (gpointer)file_widget);
	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(file_widget)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), G_OBJECT(file_widget));
	gtk_widget_show(file_widget);

	// ensure the dialog will not end up behind the editor window
	gdk_window_set_keep_above(file_widget->window, 1);
	gdk_window_raise(file_widget->window);
	gdk_window_show(file_widget->window);

	gtk_main();

	if (!tmp_filename.empty())
	{
		try
		{
			iLevel->Load(tmp_filename.c_str());
			std::string temp = getpath(tmp_filename.c_str());
			temp += "default.ini";
			std::string fname(tmp_filename.substr(0, tmp_filename.size()-4)+".ini");
			iGraphics->Load(temp.c_str(),fname.c_str());
			iGD->SetPalette(*iGraphics->Palette(),256);
		}
		catch( CFailureException& e )
		{
			error("Cannot open file (%s)!", e.what());
		}
	}
	else
	{
		// nothing
	}

	return false;
#endif	// end __unix__
}


bool CEditorLevelIO::SaveLevelAs()
{
#ifndef __unix__

	OPENFILENAME ofn;       // common dialog box structure
	char szFile[FILENAME_MAX];       // buffer for file name
	char tmp[FILENAME_MAX];
	HWND hwnd;              // owner window
	int ret;

	hwnd = iWndInfo.window;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(&szFile, sizeof(szFile));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Level (*.lev)\0*.LEV\0All (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER;

// Display the Open dialog box.
	_getcwd(tmp,FILENAME_MAX);

	if (GetSaveFileName(&ofn)==TRUE)
	{
		_chdir( tmp );
		char *tmp;

		if (stricmp(ofn.lpstrFile+strlen(ofn.lpstrFile)-4,".lev")!=0)
		{
			tmp=(char*)malloc(strlen(ofn.lpstrFile)+5);
			sprintf(tmp,"%s.lev",ofn.lpstrFile);
		}
		else
			tmp=strdup(ofn.lpstrFile);

		ASSERT( tmp );

		if (!iLevel->Save( tmp )) 
			error("Saving Failed (%s)!", tmp);
		else 
		{
			free(tmp);
			return true;
		}
	}
	else
	{
		// TODO: Do nothing 
		ret = CommDlgExtendedError();
	}

	_chdir( tmp );
	return false; // If no level saved

#else	// __unix__

	tmp_filename.clear();
	file_widget = gtk_file_selection_new("Save level as:");
	g_signal_connect(G_OBJECT(file_widget), "destroy", G_CALLBACK(gtk_main_quit), 0);
	g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(file_widget)->ok_button), "clicked", G_CALLBACK(file_callback), (gpointer)file_widget);
	g_signal_connect_swapped(G_OBJECT(GTK_FILE_SELECTION(file_widget)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), G_OBJECT(file_widget));
	gtk_widget_show(file_widget);

	// ensure the dialog will not end up behind the editor window
	gdk_window_set_keep_above(file_widget->window, 1);
	gdk_window_raise(file_widget->window);
	gdk_window_show(file_widget->window);

	gtk_main();

	if (!tmp_filename.empty())
	{
		char *tmp;

		if (tmp_filename.size()<4 || stricmp(tmp_filename.c_str()+tmp_filename.size()-4,".lev")!=0)
		{
			tmp=(char*)malloc(tmp_filename.size()+5);
			sprintf(tmp, "%s.lev", tmp_filename.c_str());
		}
		else
			tmp=strdup(tmp_filename.c_str());

		ASSERT( tmp );

		if (!iLevel->Save( tmp ))
			error("Saving Failed (%s)!", tmp);
		else 
		{
			free(tmp);
			return true;
		}
	}
	else
	{
		// void
	}

	return false;

#endif	// end __unix__
}


bool CEditorLevelIO::SaveLevel()
{
	if (*iLevel->LevelFileName() == 0)
		return SaveLevelAs();
	else 
	{
		if (! iLevel->Save(iLevel->LevelFileName()))
			error("Saving Failed (%s)!",iLevel->LevelFileName());
		else
#ifndef __unix__
			MessageBox(iWndInfo.window,"Level saved !","",MB_OK | MB_ICONINFORMATION);
#else
		{
			GtkWidget* window = gtk_dialog_new();
			g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (end_widget), NULL);
			GtkWidget* text = gtk_label_new("Level saved !");
			gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->vbox), text, TRUE, TRUE, 0);
			GtkWidget* button = gtk_button_new_with_label ("OK");
			gtk_box_pack_start (GTK_BOX (GTK_DIALOG (window)->action_area), button, TRUE, TRUE, 0);
			g_signal_connect_swapped (G_OBJECT (button), "clicked", G_CALLBACK(gtk_widget_destroy), G_OBJECT(window));
			gtk_widget_show(text);
			gtk_widget_show(button);
			gtk_widget_show(window);
			gdk_window_set_title(window->window, "TK4 Editor");

			// ensure the box will not end up behind the editor window
			gdk_window_set_keep_above(window->window, 1);
			gdk_window_raise(window->window);
			gdk_window_show(window->window);

			gtk_main();
		}
#endif

		return false;
	}
}
