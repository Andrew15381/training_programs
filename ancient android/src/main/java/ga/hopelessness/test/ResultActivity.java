package ga.hopelessness.test;

import android.os.*;
import android.support.v4.util.*;
import android.support.v7.app.*;
import android.widget.*;
import ga.hopelessness.test.*;
import ga.hopelessness.test.server.*;
import ga.hopelessness.test.constants.*;

/*Экран вывода результата*/
public final class ResultActivity extends AppCompatActivity
{
	/*Инициализирует View, выводит оценку.
	  savedInstanceState - данные для восстановления*/
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.result);

		TextView markDisplay = (TextView)findViewById(R.id.result_mark);

		final String testId = getIntent().getExtras().getString(IntentKeys.TEST_ID);
		final int points = getIntent().getExtras().getInt(IntentKeys.TEST_POINTS);

		LoadMarkAsyncTask asyncTask = new LoadMarkAsyncTask(this);
		String mark = new String();
		asyncTask.execute(new Pair<String, Integer>(testId, points));
		try
		{
			mark = asyncTask.get();
		}
		catch (Exception e)
		{
			Toast.makeText(this, "Проблема с интернетом", Toast.LENGTH_LONG).show();
		}
		markDisplay.setText(mark);
	}
}
