#include "EditorLevelIO.h"

#include "../common/CEditableLevel.h"
#include "../common/CGraphicsDevice.h"
#include "../common/error.h"
#include "../common/files.h"
#include "EditorDrawer.h"
#include "EditorGraphics.h"

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

CEditorLevelIO::CEditorLevelIO(CEditableLevel* aLevel,SDL_SysWMinfo aWndInfo,CEditorDrawer* aDrawer, CEditorGraphics* aGraphics, CGraphicsDevice* aGD)
{
	iLevel = aLevel;
	iDrawer = aDrawer;
	iWndInfo = aWndInfo;
	iGraphics = aGraphics;
	iGD = aGD;

	gtk_init(0, 0);
}

CEditorLevelIO::~CEditorLevelIO(void)
{
}

bool CEditorLevelIO::LoadINI()
{
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
}

bool CEditorLevelIO::LoadLevel()
{
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
}


bool CEditorLevelIO::SaveLevelAs()
{
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

		if (tmp_filename.size()<4 || strcasecmp(tmp_filename.c_str()+tmp_filename.size()-4,".lev")!=0)
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

		return false;
	}
}
