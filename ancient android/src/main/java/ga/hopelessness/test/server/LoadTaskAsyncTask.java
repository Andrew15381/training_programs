package ga.hopelessness.test.server;

import android.content.*;
import android.os.*;
import android.widget.*;
import ga.hopelessness.test.model.*;

/*Загрузчик заданий с сервера*/
public final class LoadTaskAsyncTask extends AsyncTask<String, Void, TaskModel>
{
	/*Контекст приложения*/
	private Context context;
	/*Исключение*/
	private Exception exception = null;

	/*Создает новый загрузчик.
	 context - контекст приложения*/
	public LoadTaskAsyncTask(Context context)
	{
		this.context = context;
	}

	/*Загружает задания с сервера.
	 themes - темы заданий*/
	@Override
	protected TaskModel doInBackground(String ...theme)
	{
		TaskModel task=new TaskModel();
		try
		{
			task = Server.loadTask(theme[0]);
		}
		catch (Exception e)
		{
			exception = e;
		}
		return task;
	}

	/*Выводит сообщение об ошибке.
	  task - загруженное задание*/
	@Override
	protected void onPostExecute(TaskModel task)
	{
		if (exception != null)
		{
			Toast.makeText(context, "Нет соединения с сервером", Toast.LENGTH_LONG).show();
		}
	}
}
