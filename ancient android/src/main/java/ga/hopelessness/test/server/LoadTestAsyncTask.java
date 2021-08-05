package ga.hopelessness.test.server;

import android.content.*;
import android.os.*;
import android.support.v7.widget.*;
import android.widget.*;
import ga.hopelessness.test.*;
import ga.hopelessness.test.model.*;
import java.util.*;

/*Загрузчик тестов с сервера*/
public final class LoadTestAsyncTask extends AsyncTask<Void, Void, ArrayList<TestModel>>
{
	/*Контекст приложения*/
	private Context context;
	/*Исключение*/
	private Exception exception = null;
	/*View для вывода тестов*/
	private RecyclerView testsDisplay;

	/*Создает новый загрузчик тестов.
	 context - контекст приложения*/
	public LoadTestAsyncTask(Context context, RecyclerView testsDisplay)
	{
		this.context = context;
		this.testsDisplay = testsDisplay;
	}

	/*Загружает тесты с сервера.*/
	@Override
	protected ArrayList<TestModel> doInBackground(Void ...p)
	{
		ArrayList<TestModel> tests = new ArrayList<TestModel>();
		try
		{
			tests = Server.loadTests();
		}
		catch (Exception e)
		{
			exception = e;
		}
		return tests;
	}

	/*Выводит сообщение об ошибке.
	  tests - загруженные тесты*/
	@Override
	protected void onPostExecute(ArrayList<TestModel> tests)
	{
		if (exception != null)
		{
			Toast.makeText(context, "Нет соединения с сервером", Toast.LENGTH_LONG).show();
		}
		else
		{
			testsDisplay.setAdapter(new SlideInAdapter(tests));
			testsDisplay.getAdapter().notifyDataSetChanged();
			testsDisplay.scheduleLayoutAnimation();
		}
	}
}
