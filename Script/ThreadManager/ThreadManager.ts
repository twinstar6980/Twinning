namespace TwinKleS {

	// ------------------------------------------------

	export class ThreadManager {

		private m_pool: Array<{
			thread: Core.Misc.Thread;
			context: Core.Misc.Context;
		}> = [];

		// ------------------------------------------------

		resize(
			size: number,
			initializer: null | (() => any) = null,
		): void {
			this.m_pool = new Array(size);
			for (let i = 0; i < size; ++i) {
				this.m_pool[i] = {
					thread: Core.Misc.Thread.default(),
					context: Core.Misc.g_context.spawn(),
				};
				if (initializer !== null) {
					this.join(initializer, i);
				}
			}
			if (initializer !== null) {
				this.wait();
			}
			return;
		}

		join(
			executor: () => any,
			index: number | null = null,
		): void {
			if (this.m_pool.length === 0) {
				executor();
			} else {
				while (index === null) {
					Core.Misc.g_context.yield();
					index = this.m_pool.findIndex((e) => (!e.context.state().value));
					if (index === -1) {
						index = null;
					}
				}
				if (this.m_pool[index].context.state().value) {
					throw new MyError(`thread is started`);
				}
				let item = this.m_pool[index];
				item.context.execute(executor, item.thread);
			}
			return;
		}

		done(
		): boolean {
			return this.m_pool.every((e) => (!e.context.state().value));
		}

		wait(
		): void {
			while (!this.done()) {
				Core.Misc.g_context.yield();
			}
			return;
		}

	}

	// ------------------------------------------------

	export const g_thread_manager = new ThreadManager();

	// ------------------------------------------------

}