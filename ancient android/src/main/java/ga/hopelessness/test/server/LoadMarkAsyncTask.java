package ga.hopelessness.test.server;

import android.content.*;
import android.os.*;
import android.support.v4.util.*;
import android.widget.*;

/*Загрузчик оценки с сервера*/
public final class LoadMarkAsyncTask extends AsyncTask<Pair<String, Integer>, Void, String>
{
	/*Контекст приложения*/
	private Context context;
	/*Исключение*/
	private Exception exception = null;

	/*Создает новый загрузчик.
	 context - контекст приложения*/
	public LoadMarkAsyncTask(Context context)
	{
		this.context = context;
	}

	/*Загружает оценку с сервера.
	 testId_and_scores - идентификатор теста (first) и баллы за него (second)*/
	@Override
	protected String doInBackground(Pair<String, Integer> ...testId_and_points)
	{
		String mark=new String();
		try
		{
			mark = Server.loadMark(testId_and_points[0].first, testId_and_points[0].second);
		}
		catch (Exception e)
		{
			exception = e;
		}
		return mark;
	}

	/*Выводит сообщение об ошибке.
	  mark - загруженная оценка*/
	@Override
	protected void onPostExecute(String mark)
	{
		if (exception != null)
		{
			Toast.makeText(context, "Нет соединения с сервером", Toast.LENGTH_LONG).show();
		}
	}
}
