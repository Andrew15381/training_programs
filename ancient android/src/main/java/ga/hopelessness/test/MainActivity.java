package ga.hopelessness.test;

import android.app.*;
import android.content.*;
import android.os.*;
import android.support.v4.view.animation.*;
import android.support.v4.widget.*;
import android.support.v7.app.*;
import android.support.v7.widget.*;
import android.view.*;
import android.view.animation.*;
import ga.hopelessness.test.*;
import ga.hopelessness.test.model.*;
import ga.hopelessness.test.server.*;
import java.util.*;
import android.widget.SearchView.*;

/*Главный экран*/
public final class MainActivity extends AppCompatActivity implements SwipeRefreshLayout.OnRefreshListener
{
	/*View для вывода списка тестов*/
	private RecyclerView testsDisplay;
	/*View для обновления списка тестов*/
	private SwipeRefreshLayout testsDisplayRefresher;

	/*Инициализирует View, назначает обработчики событий.
	 savedInstanceState - данные для восстановления*/
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

		testsDisplay = (RecyclerView)findViewById(R.id.main_rv);
		testsDisplay.setLayoutManager(new LinearLayoutManager(this));
		Animation animation=AnimationUtils.loadAnimation(this, R.anim.slide_in_right);
		animation.setInterpolator(new LinearOutSlowInInterpolator());
		testsDisplay.setLayoutAnimation(new LayoutAnimationController(animation, 0.3f));

		testsDisplayRefresher = (SwipeRefreshLayout)findViewById(R.id.main_refresh);
		testsDisplayRefresher.setOnRefreshListener(this);

		onRefresh();
    }

	/*Обновляет список тестов по событию.*/
	@Override
    public void onRefresh()
	{
        testsDisplayRefresher.setRefreshing(true);

		LoadTestAsyncTask asyncTask = new LoadTestAsyncTask(this, testsDisplay);
		asyncTask.execute();

		testsDisplayRefresher.setRefreshing(false);
    }

	/*Добавляет строку поиска в меню, назначает ей обработчик.
	 menu - меню*/
	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		getMenuInflater().inflate(R.menu.menu, menu);

		SearchManager searchManager = (SearchManager)getSystemService(Context.SEARCH_SERVICE);
		SearchView searchView = (SearchView)menu.findItem(R.id.search).getActionView();
		searchView.setSearchableInfo(searchManager.getSearchableInfo(getComponentName()));
		searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener()
			{
				@Override
				public boolean onQueryTextSubmit(String query)
				{
					SearchTestAsyncTask asyncTask = new SearchTestAsyncTask(getApplicationContext(), testsDisplay);
					asyncTask.execute(query);
					
					return true;
				}
				@Override
				public boolean onQueryTextChange(String query)
				{
					SearchTestAsyncTask asyncTask = new SearchTestAsyncTask(getApplicationContext(), testsDisplay);
					asyncTask.execute(query);
					
					return true;
				}
			});
		
        return true;
	}
}
