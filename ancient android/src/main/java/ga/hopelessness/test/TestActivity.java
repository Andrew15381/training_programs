package ga.hopelessness.test;

import android.content.*;
import android.graphics.drawable.*;
import android.os.*;
import android.support.v7.app.*;
import android.text.*;
import android.util.*;
import android.view.*;
import android.view.View.*;
import android.widget.*;
import ga.hopelessness.test.*;
import ga.hopelessness.test.model.*;
import ga.hopelessness.test.server.*;
import ga.hopelessness.test.constants.*;

/*Экран задания*/
public final class TestActivity extends AppCompatActivity
{
	/*Номер текущего вопроса, баллы за тест*/
	private int currentTaskNumber=0, points=0;
	/*Темы вопросов*/
	private String[] taskThemes;


	/*View для вывода вопроса*/
	private TextView questionDisplay;
	/*View для ввода ответа*/
	private EditText answerEditor;
	/*View для принятия введенного ответа*/
	private Button confirmationButton;

	/*Инициализирует View, начинает тест.
	 savedInstanceState - данные для восстановления*/
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.test);

		questionDisplay = (TextView)findViewById(R.id.test_text);
		answerEditor = (EditText)findViewById(R.id.test_edit);
		confirmationButton = (Button)findViewById(R.id.test_send);

		taskThemes = (String[])getIntent().getSerializableExtra(IntentKeys.TASK_THEMES);

		ask();
	}

	/*Останавливает тест при потере фокуса экраном.*/
	@Override
	protected void onPause()
	{
		super.onPause();
		finish();
	}

	/*Выводит вопрос на экран, обрабатывает ответ.*/
	private void ask()
	{
		getSupportActionBar().setTitle(String.valueOf(currentTaskNumber + 1) + "/" + String.valueOf(taskThemes.length));

		LoadTaskAsyncTask asyncTask = new LoadTaskAsyncTask(this);
		TaskModel task = new TaskModel();
		asyncTask.execute(taskThemes[currentTaskNumber++]);
		try
		{
			task = asyncTask.get();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		questionDisplay.setText(Html.fromHtml(task.getQuestion(), new Html.ImageGetter()
									{
										public Drawable getDrawable(String source)
										{
											LoadImageAsyncTask asyncTask = new LoadImageAsyncTask(getApplication());
											Drawable image = null;
											asyncTask.execute(source);
											try
											{
												image = asyncTask.get();
											}
											catch (Exception e)
											{
												e.printStackTrace();
											}

											DisplayMetrics metrics = new DisplayMetrics();
											getWindowManager().getDefaultDisplay().getMetrics(metrics);
											image.setBounds(0, 0, metrics.widthPixels, metrics.widthPixels * image.getIntrinsicHeight() / image.getIntrinsicWidth());
											return image;
										}
									}, null));
		confirmationButton.setOnClickListener(new OnSubmitClickListener(task.getAnswer(), task.getPoints()));
	}

	/*Запускает экран вывода оценки.*/
	private void showResult()
	{
		Intent startResult = new Intent(this, ResultActivity.class);
		String testId = getIntent().getExtras().getString(IntentKeys.TEST_ID);
		startResult.putExtra(IntentKeys.TEST_ID, testId);
		startResult.putExtra(IntentKeys.TEST_POINTS, points);
		startActivity(startResult);
	}

	/*Обработчик нажатий на кнопку подтверждения*/
	private class OnSubmitClickListener implements OnClickListener
	{
		/*Верный ответ*/
		private String correctAnswer;
		/*Баллы за вопрос*/
		private int taskPoints;

		/*Создает новый обработчик.
		 correctAnswer - верный ответ
		 taskPoints - баллы за вопрос*/
		public OnSubmitClickListener(String correctAnswer, int taskPoints)
		{
			this.correctAnswer = correctAnswer;
			this.taskPoints = taskPoints;
		}

		/*Проверяет ответ, аыводит следующее задание.
		 pressedButton - нажатая кнопка*/
		@Override
		public void onClick(View pressedButton)
		{
			final String answer = answerEditor.getText().toString();
			if (answer.equals(correctAnswer))
			{
				points += taskPoints;
				Toast.makeText(pressedButton.getContext(), "Правильный ответ", Toast.LENGTH_SHORT).show();
			}
			else
			{
				Toast.makeText(pressedButton.getContext(), "Неверный ответ", Toast.LENGTH_SHORT).show();
				Toast.makeText(pressedButton.getContext(), "Верный ответ: " + correctAnswer, Toast.LENGTH_SHORT).show();
			}

			if (currentTaskNumber != taskThemes.length)
			{
				ask();
			}
			else
			{
				showResult();
			}
		}
	}
}
