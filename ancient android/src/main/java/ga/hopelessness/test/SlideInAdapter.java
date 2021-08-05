package ga.hopelessness.test;

import android.content.*;
import android.support.v7.widget.*;
import android.view.*;
import android.widget.*;
import ga.hopelessness.test.*;
import ga.hopelessness.test.model.*;
import java.util.*;
import ga.hopelessness.test.constants.*;

/*Заполнитель View для вывода тестов*/
public final class SlideInAdapter extends RecyclerView.Adapter
{
	/*Список тестов*/
	private ArrayList<TestModel> tests;

	/*Владелец View для вывода теста*/
	private final class Header extends RecyclerView.ViewHolder implements View.OnClickListener
	{
		/*Кнопка для запуска теста*/
		public Button startTestButton;
		/*View для вывода данных теста*/
		public TextView testInfoDisplay;
		/*Список тем заданий*/
		public String[] taskThemes;
		/*Идентификатор теста*/
		public String testId;

		/*Сохдает нового владельца.
		 item - View для вывода теста*/
		Header(View item)
		{
			super(item);
			startTestButton = (Button)item.findViewById(R.id.header_start);
			testInfoDisplay = (TextView)item.findViewById(R.id.header_info);
		}

		/*Запускает тест при нажатии на кнопку.
		 pressedButton - нажатая кнопка*/
		@Override
		public void onClick(View pressedButton)
		{
			Intent startTest = new Intent(pressedButton.getContext(), TestActivity.class);
			startTest.putExtra(IntentKeys.TEST_ID, testId);
			startTest.putExtra(IntentKeys.TASK_THEMES, taskThemes);
			pressedButton.getContext().startActivity(startTest);
		}
	}

	/*Создает заполнитель.
	 tests - данные тестов*/
	public SlideInAdapter(ArrayList<TestModel> tests)
	{
        this.tests = tests;
    }

	/*Создает нового владельца по надобности.
	 parent - корень разметки
	 typeId - тип владельца*/
	@Override
	public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int typeId)
	{
		return new Header(LayoutInflater.from(parent.getContext()).inflate(R.layout.header, parent, false));
	}

	/*Выводит элементы на экран, задает им данные для вывода.
	 holder - владелец
	 pos - позиция владельца в списке*/
	@Override
	public void onBindViewHolder(RecyclerView.ViewHolder holder, int position)
	{
		TestModel current = tests.get(position);
		Header header = (Header)holder;
		header.testId = current.getId();
		header.startTestButton.setText(current.getName());
		header.startTestButton.setOnClickListener(header);
		header.taskThemes = current.getTaskThemes();
		header.testInfoDisplay.setText("· " + current.getDate() + "\n" +
									   "· " + current.getAuthor() + "\n" +
									   "· " + current.getDescription());
	}

	/*Возвращает количество элементов.*/
	@Override
    public int getItemCount()
	{
        return tests.size();
    }
}
